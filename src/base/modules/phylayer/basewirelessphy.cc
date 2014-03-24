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
    pcTimestamp = simTime();
    scheduleAt(simTime() + par("drawPeriod").doubleValue(), pcTimer);
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
    } else if (msg == pcTimer) {
        draw();
    } else if (msg == ccaTimer) {
        senseChannel();
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

        case CMD_PHY_CCA:
            CmdCCA *cmdCCA = check_and_cast<CmdCCA*>(cmd);
            performCCA(cmdCCA->getDuration());
            break;
    }

    delete msg;
}

void BaseWirelessPhy::handleAirFrame(AirFrame* frame)
{
    recvAirFrame(frame);
}

void BaseWirelessPhy::sendUp(cPacket* pkt)
{
    if (gate("upper$o")->isPathOK()) {
        send(pkt, "upper$o");
    } else {
        printError(ERROR, "Gate is not connected. Deleting message.");
        delete pkt;
    }
}

void BaseWirelessPhy::sendCtlUp(Command* cmd)
{
    if (gate("upperCtl$o")->isPathOK()) {
        send(cmd, "upperCtl$o");
    } else {
        printError(ERROR, "Gate is not connected. Deleting message.");
        delete cmd;
    }
}

void BaseWirelessPhy::performCCA(double duration)
{
    if (radioMode == TX || radioMode == RX) {
        scheduleAt(simTime() + duration, ccaTimer);
    } else {
        printError(ERROR, "Cannot perform CCA when radio is not in TX or RX modes");
    }
}

void BaseWirelessPhy::senseChannel()
{
    bool clearChannel = false;
    if (phyEntry != NULL && phyEntry->getChannelState() == 0) clearChannel = true;

    CmdCCAR *cmd = new CmdCCAR();
    cmd->setClearChannel(clearChannel);

    sendCtlUp(cmd);
}

void BaseWirelessPhy::fetchPacket()
{
    Command *cmd = new Command();
    cmd->setCmdId(CMD_DATA_FETCH);
    sendCtlUp(cmd);
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
    // Send ready command to upper layer
    Command *cmd = new Command();
    cmd->setSrc(PHYS);
    cmd->setDes(LINK);
    cmd->setCmdId(CMD_READY);
    sendCtlUp(cmd);
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

    cPacket *pkt = frame->decapsulate();
    pkt->setBitError(frame->hasBitError());
    sendUp(pkt);

    delete frame;
}

void BaseWirelessPhy::switchRadioMode(int mode)
{
    double switchDelay = 0;

    cancelEvent(switchIdleTimer);
    cancelEvent(switchRxTimer);
    cancelEvent(switchTxTimer);

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
    // Draw energy before switching mode
    draw();

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
}

void BaseWirelessPhy::draw()
{
    double p = 0; // Power
    double energy = 0; // Consumed energy in (mWh)

    // Calculate consumed energy
    switch (radioMode) {
        case IDLE:
            p = par("pIdle").doubleValue();
            break;
        case RX:
            p = par("pRx").doubleValue();
            break;
        case TX:
            p = par("pTx").doubleValue();
            break;
    }
    energy = p * (simTime() - pcTimestamp).dbl() / 3600;

    // TODO Draw from power source

    // Update time stamp
    pcTimestamp = simTime();
    // Set pcTimer
    if (!pcTimer->isScheduled()) {
        scheduleAt(simTime() + par("drawPeriod").doubleValue(), pcTimer);
    }
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
    pcTimer = new cMessage("PcTimer");
    ccaTimer = new cMessage("CCATimer");
}

BaseWirelessPhy::~BaseWirelessPhy()
{
    cancelAndDelete(finishTxTimer);
    cancelAndDelete(fetchTimer);
    cancelAndDelete(switchTxTimer);
    cancelAndDelete(switchRxTimer);
    cancelAndDelete(switchIdleTimer);
    cancelAndDelete(pcTimer);
    cancelAndDelete(ccaTimer);
}

}
