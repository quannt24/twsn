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

#include "linkxtmac.h"
#include "netpkt_m.h"
#include "stathelper.h"
#include "base802154phy.h"

namespace twsn {

Define_Module(LinkXTMAC);

void LinkXTMAC::initialize()
{
    // Just copy LinkUnslottedCSMACA::initialize() without turning transceiver to RX mode
    BaseLink::initialize();
    aMaxBE = par("aMaxBE").longValue();
    macMaxNB = par("macMaxNB").longValue();
    macMinBE = par("macMinBE").longValue();
    ifsLen = par("aMinLIFSPeriod").doubleValue();
}

void LinkXTMAC::handleSelfMsg(cMessage* msg)
{
    if (msg == backoffTimer) {
        performCCA();
    } else if (msg == listenTimer) {
        // Note this is LinkUnslottedCSMACA's timer, it is not used here.
        /*// Switch radio transceiver to listen mode
        Command *cmd = new Command();
        cmd->setSrc(LINK);
        cmd->setDes(PHYS);
        cmd->setCmdId(CMD_PHY_RX);
        sendCtlDown(cmd);*/
    } else if (msg == ifsTimer) {
        // Send packet at head of the queue if ready
        prepareQueuedPkt();
    } else if (msg == strobeTimer) {
        if (nStrobe > 0) {
            // Send next strobe
            sendStrobe();
        } else {
            std::cerr << "Finish sending strobes t=" << simTime() << endl;
            delete strobePkt;
            strobePkt = NULL;

            // Send main packet anyway
            outPkt = mainPkt;
            notifyLower();
        }
    } else if (msg == deadlineTimer) {
        if (nStrobe > 0 && strobePkt != NULL) {
            // Cancel current strobe sending
            printError(INFO, "Strobe deadline is missed");
            nStrobe--;
            if (outPkt != NULL && outPkt->getOwner() == this) {
                // Delete strobe if it has not being sent yet
                delete outPkt;
                outPkt = NULL;
            }
            cancelEvent(backoffTimer);

            // Switch radio transceiver to listen mode
            switchToRx();
        }
    } else if (msg == mainSendingTimer) {
        if (mainPkt != NULL) {
            if (outPkt == NULL) {
                printError(INFO, "Send main packet");
                outPkt = mainPkt;
                notifyLower();
            } else {
                printError(ERROR, "Not ready for sending");
                delete mainPkt;
                mainPkt = NULL;

                // Count packet loss
                StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
                if (mainPkt->getPktType() == MAC802154_DATA) sh->countLostMacPkt();
            }
        } else {
            printError(WARNING, "NULL main packet, prepare other");
            prepareQueuedPkt();
        }
    } else if (msg == dcSleepTimer) {
        active = false;
        forcedActive = false;
        switchToIdle();
        // Plan a listen timer
        cancelEvent(dcListenTimer);
        scheduleAt(simTime() + par("sleepInterval").doubleValue(), dcListenTimer);
    } else if (msg == dcListenTimer) {
        // Switch radio transceiver to RX mode if currently in IDLE
        switchToRx();
        // Plan a sleep timer
        cancelEvent(dcSleepTimer);
        scheduleAt(simTime() + par("listenInterval").doubleValue(), dcSleepTimer);
    }
}

void LinkXTMAC::handleUpperMsg(cMessage* msg)
{
    // TODO Limit packet size

    /* Encapsulate then enqueue packet from upper layer */
    NetPkt *netpkt = check_and_cast<NetPkt*>(msg);
    Mac802154Pkt *macpkt = new Mac802154Pkt();

    // TODO Address resolution
    macpkt->setSrcAddr(netpkt->getSrcAddr());
    macpkt->setDesAddr(netpkt->getDesAddr());

    macpkt->setByteLength(macpkt->getPktSize());
    macpkt->encapsulate(netpkt);

    outQueue.insert(macpkt);
    activate();

    // Send packet at head of the queue if ready
    prepareQueuedPkt();
}

void LinkXTMAC::handleUpperCtl(cMessage* msg)
{
    // Copy LinkUnslottedCSMACA::handleUpperCtl and add more functions
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_LIN_FORCE_ACTIVE: {
            CmdForceActive *cfa = check_and_cast<CmdForceActive*>(cmd);
            activate(true, cfa->getDuration());
            delete cmd;
            break;
        }

        default:
            // Just forward to lower layer
            if (cmd->getDes() != LINK) {
                sendCtlDown(cmd);
            } else {
                printError(WARNING, "Unknown command from upper");
                delete cmd; // Unknown command
            }
            break;
    }
}

void LinkXTMAC::handleLowerMsg(cMessage* msg)
{
    Mac802154Pkt *macpkt = check_and_cast<Mac802154Pkt*>(msg);
    cPacket *netpkt = NULL;
    StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));

    if (macpkt->hasBitError()) {
        getParentModule()->bubble("Packet error");
        // Count packet loss
        if (macpkt->getPktType() == MAC802154_DATA) sh->countLostMacPkt();
        delete macpkt;
        return;
    }

    // Count received packet
    if (macpkt->getPktType() == MAC802154_DATA) {
        sh->countRecvMacPkt();
    }

    switch (macpkt->getPktType()) {
        case MAC802154_DATA:
            activate();
            // Forward network packet to upper layer
            netpkt = macpkt->decapsulate();
            if (netpkt != NULL) {
                sendUp(netpkt);
            } else {
                printError(WARNING, "NULL payload");
            }
            delete macpkt;
            break;

        case MAC802154_PREAMBLE:
            printError(INFO, "Strobe received");
            if (macpkt->getDesAddr() == macAddr) {
                activate();
                sendAck(macpkt->getSrcAddr());
            } else if (macpkt->getDesAddr() == MAC_BROADCAST_ADDR) {
                activate();
                // We do not send ACK to broadcasted strobes
            }
            delete macpkt;
            break;

        case MAC802154_ACK:
            printError(INFO, "ACK received");
            delete macpkt;
            activate();

            // Cancel strobing
            nStrobe = 0;
            if (strobePkt != NULL) {
                delete strobePkt;
                strobePkt = NULL;
            }
            cancelEvent(strobeTimer);
            cancelEvent(deadlineTimer);
            if (outPkt != NULL && outPkt->getPktType() == MAC802154_PREAMBLE) {
                cancelEvent(backoffTimer);
                delete outPkt;
                outPkt = NULL;
            }
            if (outPkt == NULL) {
                // Send main packet if not sending non-preamble packet
                scheduleAt(simTime() + ifsLen, mainSendingTimer);
            }
            break;

        default:
            printError(WARNING, "Unknown MAC packet type");
            // Count packet loss
            StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
            if (macpkt->getPktType() == MAC802154_DATA) sh->countLostMacPkt();
            delete macpkt;
            break;
    }
}

void LinkXTMAC::reset()
{
    if (outPkt == mainPkt) {
        // Reset mainPkt pointer so that we can send next packet
        mainPkt = NULL;
    } else {
        // Strobe sent
        nStrobe--;
        cancelEvent(deadlineTimer);
    }

    // Reset outPkt pointer
    outPkt = NULL;

    cancelEvent(ifsTimer);
    scheduleAt(simTime() + ifsLen, ifsTimer);

    // Switch radio transceiver to listen mode
    Command *rxcmd = new Command();
    rxcmd->setSrc(LINK);
    rxcmd->setDes(PHYS);
    rxcmd->setCmdId(CMD_PHY_RX);
    sendCtlDown(rxcmd);
}

void LinkXTMAC::activate(bool forced, double duration)
{
    // Switch radio transceiver to RX mode if currently in IDLE
    switchToRx();

    active = true;
    if (forced) {
        if (duration > 0) {
            cancelEvent(dcSleepTimer);
            scheduleAt(simTime() + duration, dcSleepTimer);
        }
        forcedActive = true;
    } else {
        if (!forcedActive) {
            // If not in forcedActive interval then set dcSleepTimer as normal. If in forcedActive
            // interval, dcSleepTimer is already set and we will not change it.
            cancelEvent(dcSleepTimer);
            scheduleAt(simTime() + par("activeTime"), dcSleepTimer);
        }
    }
}

void LinkXTMAC::prepareQueuedPkt()
{
    if (nStrobe <= 0
            && mainPkt == NULL
            && !ifsTimer->isScheduled()
            && !mainSendingTimer->isScheduled()
            && !outQueue.empty()) {

        mainPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());

        if (mainPkt->getPktType() == MAC802154_DATA) {
            NetPkt *encPkt = check_and_cast<NetPkt*>(mainPkt->getEncapsulatedPacket());

            if (encPkt->getPreambleFlag()) {
                // Prepare strobes
                nStrobe = (int) ceil(par("sleepInterval").doubleValue() / par("strobePeriod").doubleValue());
                std::cerr << "Sending " << nStrobe << " strobes t=" << simTime() << endl;

                strobePkt = new Mac802154Pkt();
                strobePkt->setPktType(MAC802154_PREAMBLE);
                strobePkt->setSrcAddr(this->macAddr);
                strobePkt->setDesAddr(mainPkt->getDesAddr());
                strobePkt->setByteLength(strobePkt->getPktSize());

                // Send strobe
                sendStrobe();
            } else {
                // Send main packet immediately
                nStrobe = 0;
                outPkt = mainPkt;
                notifyLower();
            }
        } else {
            // Send main packet immediately
            nStrobe = 0;
            outPkt = mainPkt;
            notifyLower();
        }
    }
}

void LinkXTMAC::sendStrobe()
{
    if (outPkt == NULL && !ifsTimer->isScheduled() && nStrobe > 0 && strobePkt != NULL) {
        outPkt = strobePkt->dup();
        notifyLower();
        // Set deadline
        scheduleAt(simTime() + par("strobeDeadline").doubleValue(), deadlineTimer);
        // Set timer for next period
        scheduleAt(simTime() + par("strobePeriod").doubleValue(), strobeTimer);
    }
}

void LinkXTMAC::sendAck(macaddr_t addr)
{
    Mac802154Pkt *ack = new Mac802154Pkt();
    ack->setPktType(MAC802154_ACK);
    ack->setSrcAddr(macAddr);
    ack->setDesAddr(addr);
    ack->setByteLength(ack->getPktSize());

    outQueue.insert(ack);

    // Send packet at head of the queue if ready
    prepareQueuedPkt();
}

void LinkXTMAC::switchToRx()
{
    Base802154Phy *phy = check_and_cast<Base802154Phy*>(getModuleByPath("^.phy"));
    if (phy->getRadioMode() == IDLE) {
        Command *cmd = new Command();
        cmd->setSrc(LINK);
        cmd->setDes(PHYS);
        cmd->setCmdId(CMD_PHY_RX);
        sendCtlDown(cmd);
    }
}

void LinkXTMAC::switchToIdle()
{
    Base802154Phy *phy = check_and_cast<Base802154Phy*>(getModuleByPath("^.phy"));
    if (phy->getRadioMode() != IDLE) {
        Command *cmd = new Command();
        cmd->setSrc(LINK);
        cmd->setDes(PHYS);
        cmd->setCmdId(CMD_PHY_IDLE);
        sendCtlDown(cmd);
    }
}

LinkXTMAC::LinkXTMAC()
{
    mainPkt = NULL;
    nStrobe = 0;
    active = false;
    forcedActive = false;
    deadlineTimer = new cMessage("deadlineTimer");
    strobeTimer = new cMessage("strobeTimer");
    mainSendingTimer = new cMessage("mainSendingTimer");
    dcSleepTimer = new cMessage("dcSleepTimer");
    dcListenTimer = new cMessage("dcListenTimer");
}

LinkXTMAC::~LinkXTMAC()
{
    cancelAndDelete(deadlineTimer);
    cancelAndDelete(strobeTimer);
    cancelAndDelete(mainSendingTimer);
    cancelAndDelete(dcSleepTimer);
    cancelAndDelete(dcListenTimer);
}

}  // namespace twsn
