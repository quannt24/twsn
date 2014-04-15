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
#include "baseenergy.h"
#include "coord.h"
#include "command_m.h"
#include "stathelper.h"

namespace twsn {

Define_Module(BaseWirelessPhy);

void BaseWirelessPhy::initialize()
{
    // Stage 0
    // Call initialize() of parent
    BasePhy::initialize();
    radioMode = par("initRadioMode").longValue();
    pcTimestamp = simTime();

    std::ostringstream oss;
    oss << par("txRange").doubleValue();
    std::string str = oss.str();
    str.copy(rangeStr, str.length());
    rangeStr[str.length()] = '\0';

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
    } else if (msg == txTimer) {
        txMacPkt(check_and_cast<Mac802154Pkt*>((Mac802154Pkt*) msg->getContextPointer()));
    }
}

void BaseWirelessPhy::handleUpperMsg(cMessage* msg)
{
    /* Transmit MAC packet. The packet only transmitted if physical module ready; if not, it will
     * be deleted. To make sure the packet is transmitted (or canceled with report), link layer
     * must follow normal sending procedures instead of sending the packet immediately. */

    if (!finishTxTimer->isScheduled()) {
        if (radioMode == TX) {
            txMacPkt(check_and_cast<Mac802154Pkt*>(msg));
        } else {
            // Switch to TX mode then transmit
            if (!switchTxTimer->isScheduled()) switchRadioMode(TX);
            // Transmit when switched to TX
            txTimer->setContextPointer(msg); // Save packet pointer for later use
            scheduleAt(switchTxTimer->getArrivalTime(), txTimer);
        }
    } else {
        Mac802154Pkt *pkt = check_and_cast<Mac802154Pkt*>(msg);
        if (pkt->getPktType() != MAC802154_PREAMBLE) {
            // Count packet loss
            StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
            sh->countLostMacPkt();
        }
        delete msg;
    }
}

void BaseWirelessPhy::handleUpperCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_PHY_PD:
            setRadioMode(POWER_DOWN);
            break;

        case CMD_PHY_IDLE:
            switchRadioMode(IDLE);
            break;

        case CMD_PHY_RX:
            switchRadioMode(RX);
            break;

        case CMD_PHY_TX:
            switchRadioMode(TX);
            break;

        case CMD_DATA_NOTI:
            if (!finishTxTimer->isScheduled()) {
                // It is ready for sending. Send a fetch command immediately
                fetchPacket();
            } else {
                // Fetch data when current tx completes
                if (!fetchTimer->isScheduled()) {
                    scheduleAt(finishTxTimer->getArrivalTime(), fetchTimer);
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
    if (radioMode == RX) {
        scheduleAt(simTime() + duration, ccaTimer);
    } else {
        // CCA is only valid in RX mode, send back a busy result
        CmdCCAR *cmd = new CmdCCAR();
        cmd->setClearChannel(false);
        sendCtlUp(cmd);

        printError(WARNING, "Cannot perform CCA when radio is not in RX mode");
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

void BaseWirelessPhy::txMacPkt(Mac802154Pkt* pkt)
{
    if (channelMgr == NULL || phyEntry == NULL) {
        printError(ERROR, "Module has not registered with ChannelMgr. Dropping packet.");
        delete pkt;
        return;
    }
    if (radioMode != TX) {
        printError(WARNING, "Cannot transmit when not in TX mode. Dropping packet.");
        // Count packet loss
        StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
        if (pkt->getPktType() != MAC802154_PREAMBLE) sh->countLostMacPkt();
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

    /* Set timer for calling finishTx(), simulate transmission process */
    scheduleAt(simTime() + txTime, finishTxTimer);
    updateNodeDisplay();

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
}

void BaseWirelessPhy::finishTx()
{
    channelMgr->stopTx(phyEntry);
    updateNodeDisplay();

    // Send ready command to upper layer
    Command *cmd = new Command();
    cmd->setSrc(PHYS);
    cmd->setDes(LINK);
    cmd->setCmdId(CMD_READY);
    sendCtlUp(cmd);
}

void BaseWirelessPhy::sendAirFrame(AirFrame* frame)
{
    // Check if receiver is an adjacent node
    std::list<PhyEntry*> *adjList = phyEntry->getAdjList();

    for (std::list<PhyEntry*>::iterator adjIt = adjList->begin(); adjIt != adjList->end(); adjIt++) {
        if ((*adjIt)->getModuleId() == frame->getReceiver()) {
            // Check if receiver is having RX radio mode; if not, mark frame with error
            BaseWirelessPhy *recvPhy = check_and_cast<BaseWirelessPhy*>(simulation.getModule(frame->getReceiver()));
            if (recvPhy->getRadioMode() != RX) frame->setBitError(true);

            // Send frame to receiver
            simtime_t txTime = ((double) frame->getBitLength()) / par("bitRate").doubleValue();
            channelMgr->holdAirFrame(phyEntry, frame);
            sendDirect(frame, 0, txTime, simulation.getModule(frame->getReceiver()), "radioIn");
            return;
        }
    }

    /* Receiver is out-ranged. Transmission (in this node) is still simulated, but frame will be
     * dropped here.*/
    printError(ERROR, "Cannot send to an out-ranged node. Dropping frame.");
    Mac802154Pkt *pkt = check_and_cast<Mac802154Pkt*>(frame->getEncapsulatedPacket());
    // Count packet loss
    StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
    if (pkt->getPktType() != MAC802154_PREAMBLE) sh->countLostMacPkt();
    delete frame;
}

void BaseWirelessPhy::recvAirFrame(AirFrame* frame)
{
    if (channelMgr == NULL) {
        printError(ERROR, "Module has not registered with ChannelMgr. Dropping frame.");
        delete frame;
        return;
    }

    channelMgr->releaseAirFrame(frame); // Remove frame from management list of ChannelMgr

    if (radioMode != RX) {
        printError(INFO, "Cannot receive when not in RX mode. Dropping frame.");
        Mac802154Pkt *pkt = check_and_cast<Mac802154Pkt*>(frame->getEncapsulatedPacket());
        // Count packet loss
        StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
        if (pkt->getPktType() != MAC802154_PREAMBLE) sh->countLostMacPkt();
        delete frame;
        return;
    }

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

    if (radioMode != IDLE && radioMode != RX && radioMode != TX) {
        printError(WARNING, "Cannot switch radio mode when power down");
        return;
    }

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
            printError(ERROR, "Unexpected radio mode. Set to POWER_DOWN.");
            setRadioMode(POWER_DOWN);
            return;
    }
}

void BaseWirelessPhy::setRadioMode(int mode)
{
    // Draw energy before switching mode
    BaseEnergy *ener = check_and_cast<BaseEnergy*>(getModuleByPath("^.energy"));
    if (ener->par("hasLinePower").boolValue() || ener->getCapacity() > 0) {
        draw(); // Check for capacity before call draw() to prevent infinite loop
    } else {
        radioMode = POWER_DOWN;
        return;
    }

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

    updateNodeDisplay();

    if (radioMode == POWER_DOWN) {
        getParentModule()->bubble("POWER_DOWN");
        // Cancel timers
        cancelEvent(fetchTimer);
        cancelEvent(switchTxTimer);
        cancelEvent(switchRxTimer);
        cancelEvent(switchIdleTimer);
        cancelEvent(pcTimer);
        cancelEvent(ccaTimer);
    }
}

void BaseWirelessPhy::updateNodeDisplay()
{
    cDisplayString &ds = getParentModule()->getDisplayString();

    // Set color according to radio mode
    switch (radioMode) {
        case TX:
            ds.setTagArg("i", 1, "green");
            break;
        case RX:
            ds.setTagArg("i", 1, "yellow");
            break;
        case IDLE:
            ds.setTagArg("i", 1, "grey");
            break;
        default:
            ds.setTagArg("i", 1, "black");
            break;
    }

    // Draw range if transmitting
    if (finishTxTimer->isScheduled()) {
        ds.setTagArg("r", 0, rangeStr);
    } else {
        ds.removeTag("r");
    }
}

void BaseWirelessPhy::draw()
{
    double p = 0; // Power
    double drawAmount = 0; // Consumed energy in (mWh)

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
    drawAmount = p * (simTime() - pcTimestamp).dbl() / 3600;

    // Draw from power source
    BaseEnergy *ener = check_and_cast<BaseEnergy*>(getModuleByPath("^.energy"));
    ener->draw(drawAmount);

    // Update time stamp
    pcTimestamp = simTime();

    cancelEvent(pcTimer);
    if (ener->par("hasLinePower").boolValue() || ener->getCapacity() > 0) {
        // Reset pcTimer
        scheduleAt(simTime() + par("drawPeriod").doubleValue(), pcTimer);
    } else {
        setRadioMode(POWER_DOWN); // Run out of energy
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
    txTimer = new cMessage("txTimer");
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
    cancelAndDelete(txTimer);
}

}
