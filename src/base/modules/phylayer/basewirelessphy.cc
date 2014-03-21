//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "basewirelessphy.h"
#include "channelmgr.h"
#include "basemobility.h"
#include "coord.h"
#include "command_m.h"

namespace twsn {

Define_Module(BaseWirelessPhy);

void BaseWirelessPhy::initialize()
{
    // Stage 0
    // Call initialize() of parent
    BasePhy::initialize();
    radioMode = par("initRadioMode").longValue();
}

void BaseWirelessPhy::initialize(int stage)
{
    //EV << "BaseWirelessPhy::info: initialization stage " << stage << endl;
    switch (stage) {
        case 0:
            initialize();
            break;
        case 1:
            // Register with ChannelMgr at stage 1
            registerChannel();
            break;
    }
}

void BaseWirelessPhy::registerChannel()
{
    channelMgr = check_and_cast<ChannelMgr*>(getModuleByPath("channelMgr"));
    BaseMobility *mob = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));
    if (channelMgr == NULL || mob == NULL) return;

    phyEntry = channelMgr->registerChannel(getId(), mob->getCoord(), (distance_t) par("txRange").doubleValue());
    EV << "BaseWirelessPhy::info: Register channel\n";
}

void BaseWirelessPhy::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGate() == gate("upper$i")) {
        handleUpperMsg(msg);
    } else if (msg->getArrivalGate() == gate("upperCtl$i")) {
        handleUpperCtl(msg);
    } else if (msg->getArrivalGate() == gate("radioIn")) {
        handleAirFrame(check_and_cast<AirFrame*>(msg));
    }
}

void BaseWirelessPhy::handleSelfMsg(cMessage* msg)
{
    if (msg == fetchTimer) {
        fetchPacket();
    } else if (msg == finishTxTimer) {
        finishTx();
    } else if (msg == switchTxTimer) {
        setRadioMode(TX);
    } else if (msg == switchRxTimer) {
        setRadioMode(RX);
    } else if (msg == switchIdleTimer) {
        setRadioMode(IDLE);
    }
}

void BaseWirelessPhy::handleUpperMsg(cMessage* msg)
{
    /* Transmit MAC packet. The packet only transmitted if physical module ready; if not, it will
     * be deleted. To make sure the packet is transmitted (or canceled with report), link layer
     * must follow normal sending procedures instead of sending the packet immediately. */

    if (radioMode == TX && !finishTxTimer->isScheduled()) {
        txMacPkt(check_and_cast<MacPkt*>(msg));
    } else {
        delete msg;
    }
}

void BaseWirelessPhy::handleUpperCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_NOTI:
            if (radioMode == TX) {
                if (!finishTxTimer->isScheduled()) {
                    // It is ready for sending. Send a fetch command immediately
                    fetchPacket();
                } else {
                    // Fetch data when current tx completes
                    if (!fetchTimer->isScheduled()) {
                        scheduleAt(finishTxTimer->getArrivalTime(), fetchTimer);
                    }
                }
            } else {
                // Switch to TX mode. If receiving a frame, it may be interfered.
                switchRadioMode(TX);
                // Fetch data when current tx completes
                if (!fetchTimer->isScheduled()) {
                    scheduleAt(switchTxTimer->getArrivalTime(), fetchTimer);
                }
            }
            break;
    }

    delete msg;
}

void BaseWirelessPhy::handleAirFrame(AirFrame* frame)
{
    recvAirFrame(frame);
}

void BaseWirelessPhy::fetchPacket()
{
    // TODO Send fetch command
}

void BaseWirelessPhy::txMacPkt(MacPkt* pkt)
{
    if (phyEntry == NULL) {
        std::cerr << "BaseWirelessPhy::error: module has not registered with ChannelMgr\n";
        delete pkt;
        return;
    }
    /* Increase channel state */
    channelMgr->startTx(phyEntry);

    /* Create physical frame */
    AirFrame *frame = new AirFrame();
    frame->setByteLength(frame->getFrameSize()); // Physical frame size only
    frame->encapsulate(pkt); // Frame length will be increased by payload
    frame->setSender(getId());

    /* Calculate transmission time */
    simtime_t txTime = ((double) frame->getBitLength()) / par("bitRate").doubleValue();

    /* Send air frame containing MAC packet to each receiver */
    if (pkt->getDesAddr() == MAC_BROADCAST_ADDR) {
        // Send to all adjacent nodes
        std::list<PhyEntry*>::iterator adjIt;
        AirFrame *copy;

        for (adjIt = phyEntry->getAdjList()->begin(); adjIt != phyEntry->getAdjList()->end(); adjIt++) {
            copy = frame->dup();
            copy->setReceiver((*adjIt)->getModuleId());
            sendAirFrame(copy);
        }
        delete frame; // Original frame is redundant
    } else {
        frame->setReceiver(pkt->getDesAddr()); // MAC address is module id of physical module
        sendAirFrame(frame);
        // We do not simulate overhearing
    }

    /* Set timer for calling finishTx() */
    scheduleAt(simTime() + txTime, finishTxTimer);
}

void BaseWirelessPhy::finishTx()
{
    channelMgr->stopTx(phyEntry);
    // Auto switch to Rx
    if (!fetchTimer->isScheduled()) {
        switchRadioMode(RX);
    }
}

void BaseWirelessPhy::sendAirFrame(AirFrame* frame)
{
    simtime_t txTime = ((double) frame->getBitLength()) / par("bitRate").doubleValue();
    channelMgr->holdAirFrame(phyEntry, frame);
    sendDirect(frame, 0, txTime, simulation.getModule(frame->getReceiver()), "radioIn");
}

void BaseWirelessPhy::recvAirFrame(AirFrame* frame)
{
    channelMgr->releaseAirFrame(frame);
    delete frame; // TODO Decapsulate the frame and processing instead of just delete it
}

void BaseWirelessPhy::switchRadioMode(int mode)
{
    double switchDelay = 0;

    switch (mode) {
        case IDLE:
            if (radioMode == TX) {
                switchDelay = par("delayTxToIdle").doubleValue();
            } else if (radioMode == RX) {
                switchDelay = par("delayRxToIdle").doubleValue();
            }
            scheduleAt(simTime() + switchDelay, switchIdleTimer);
            break;
        case RX:
            if (radioMode == IDLE) {
                switchDelay = par("delayIdleToRx").doubleValue();
            } else if (radioMode == TX) {
                switchDelay = par("delayTxToRx").doubleValue();
            }
            scheduleAt(simTime() + switchDelay, switchRxTimer);
            break;
        case TX:
            if (radioMode == IDLE) {
                switchDelay = par("delayIdleToTx").doubleValue();
            } else if (radioMode == RX) {
                switchDelay = par("delayRxToTx").doubleValue();
            }
            scheduleAt(simTime() + switchDelay, switchTxTimer);
            break;
        default:
            printError(ERROR, "Unexpected radio mode");
            return;
    }
}

void BaseWirelessPhy::setRadioMode(int mode)
{
    switch (mode) {
        case POWER_DOWN:
        case IDLE:
        case RX:
        case TX:
            radioMode = mode;
            break;
        default:
            printError(ERROR, "Unexpected radio mode");
            break;
    }
    // TODO Calculate power consumption
}

BaseWirelessPhy::BaseWirelessPhy()
{
    channelMgr = NULL;
    phyEntry = NULL;

    finishTxTimer = new cMessage("finishTxTimer");
    fetchTimer = new cMessage("fetchTimer");
    switchTxTimer = new cMessage("switchTxTimer");
    switchRxTimer = new cMessage("switchRxTimer");
    switchIdleTimer = new cMessage("switchIdleTimer");
}

BaseWirelessPhy::~BaseWirelessPhy()
{
    cancelAndDelete(finishTxTimer);
    cancelAndDelete(fetchTimer);
    cancelAndDelete(switchTxTimer);
    cancelAndDelete(switchRxTimer);
    cancelAndDelete(switchIdleTimer);
}

}
