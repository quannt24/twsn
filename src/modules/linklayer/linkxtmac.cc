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

    enableDutyCycling = par("enableDutyCycling").boolValue();

    // Plan initial duty cycling listen
    scheduleAt(par("sleepInterval").doubleValue(), dcListenTimer);
}

void LinkXTMAC::handleSelfMsg(cMessage* msg)
{
    StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));

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
            outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
            mainPkt = NULL;
            notifyLower();
        }
    } else if (msg == deadlineTimer) {
        if (nStrobe > 0 && strobePkt != NULL) {
            delete strobePkt;
            strobePkt = NULL;
            // Cancel current strobe sending
            printError(LV_INFO, "Strobe deadline is missed");
            nStrobe--;
            if (outPkt != NULL
                    && outPkt->getPktType() == MAC802154_PREAMBLE
                    && !transmitting) {
                // Delete strobe if it has not being sent yet
                cancelEvent(backoffTimer);
                delete outPkt;
                outPkt = NULL;
            }

            // Switch radio transceiver to listen mode
            switchToRx();
        }
    } else if (msg == mainSendingTimer) {
        if (mainPkt != NULL) {
            if (outPkt == NULL && !transmitting) {
                printError(LV_INFO, "Send main packet");
                outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
                mainPkt = NULL;
                notifyLower();
            }
        } else {
            printError(LV_WARNING, "NULL main packet, prepare other");
            prepareQueuedPkt();
        }
    } else if (msg == dcListenTimer) {
        // Switch radio transceiver to RX mode if currently in IDLE
        switchToRx();
        // Plan a sleep timer
        cancelEvent(dcSleepTimer);
        if (enableDutyCycling) {
            scheduleAt(simTime() + par("listenInterval").doubleValue(), dcSleepTimer);
        }
    } else if (msg == dcSleepTimer) {
        if (enableDutyCycling) {
            active = false;
            forcedActive = false;
            switchToIdle();
            // Plan a listen timer
            cancelEvent(dcListenTimer);
            scheduleAt(simTime() + par("sleepInterval").doubleValue(), dcListenTimer);
        }
    } else if (msg == sendTimeout) {
        // Cancel current prepared sending when it takes too long
        if (!outQueue.isEmpty()) {
            mainPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
            // Record packet loss
            if (mainPkt->getPktType() == MAC802154_DATA) sh->countLostMacPkt();

            // Discard packet
            delete mainPkt;
            mainPkt = NULL;
            nStrobe = 0;
            if (strobePkt != NULL) {
                delete strobePkt;
                strobePkt = NULL;
            }
            cancelEvent(strobeTimer);
            cancelEvent(deadlineTimer);
        }
        // Process next packet
        prepareQueuedPkt();
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
                printError(LV_WARNING, "Unknown command from upper");
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
                printError(LV_WARNING, "NULL payload");
            }
            delete macpkt;
            break;

        case MAC802154_PREAMBLE:
            printError(LV_INFO, "Strobe received");
            if (macpkt->getDesAddr() == macAddr) {
                activate();
                // Cancel current prepared sending so that we can send ACK immediately
                mainPkt = NULL;
                nStrobe = 0;
                if (strobePkt != NULL) {
                    delete strobePkt;
                    strobePkt = NULL;
                }
                cancelEvent(strobeTimer);
                cancelEvent(deadlineTimer);
                // Send back an ACK
                sendAck(macpkt->getSrcAddr());
            } else if (macpkt->getDesAddr() == MAC_BROADCAST_ADDR) {
                activate();
                // We do not send ACK to broadcasted strobes
            }
            delete macpkt;
            break;

        case MAC802154_ACK:
            printError(LV_INFO, "ACK received");
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
            if (outPkt != NULL
                    && outPkt->getPktType() == MAC802154_PREAMBLE
                    && !transmitting) {
                // Delete strobe if it has not being sent yet
                cancelEvent(backoffTimer);
                delete outPkt;
                outPkt = NULL;
            }
            if (outPkt == NULL && !transmitting) {
                // Send main packet if not sending non-preamble packet
                scheduleAt(simTime() + ifsLen, mainSendingTimer);
            }
            break;

        default:
            printError(LV_WARNING, "Unknown MAC packet type");
            // Count packet loss
            StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
            sh->countLostMacPkt();
            delete macpkt;
            break;
    }
}

void LinkXTMAC::reset()
{
    cancelEvent(deadlineTimer); // In case we are sending strobe
    cancelEvent(sendTimeout); // Unset sending timeout when sending completes

    // Reset outPkt pointer
    outPkt = NULL;
    transmitting = false;

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
        cancelEvent(dcSleepTimer);
        cancelEvent(dcListenTimer);
        if (duration > 0 && enableDutyCycling) {
            scheduleAt(simTime() + duration, dcSleepTimer);
        }
        forcedActive = true;
    } else {
        if (!forcedActive && enableDutyCycling) {
            // If not in forcedActive interval then set dcSleepTimer as normal. If in forcedActive
            // interval, dcSleepTimer is already set and we will not change it.
            cancelEvent(dcSleepTimer);
            cancelEvent(dcListenTimer);
            scheduleAt(simTime() + par("activeTime").doubleValue(), dcSleepTimer);
        }
    }
}

void LinkXTMAC::prepareQueuedPkt()
{
    if (nStrobe <= 0
            && mainPkt == NULL
            && outPkt == NULL
            && !transmitting
            && !ifsTimer->isScheduled()
            && !mainSendingTimer->isScheduled()
            && !outQueue.empty()) {

        mainPkt = check_and_cast<Mac802154Pkt*>(outQueue.front());
        // Set a timeout
        if (!sendTimeout->isScheduled()) {
            scheduleAt(simTime() + par("sendTimeout").doubleValue(), sendTimeout);
        }

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
                outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
                mainPkt = NULL;
                notifyLower();
            }
        } else {
            // Send main packet immediately
            nStrobe = 0;
            outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
            mainPkt = NULL;
            notifyLower();
        }
    } else if (outPkt != NULL) {
        // Notify lower layer in case sending is interrupted by receiving
        notifyLower();
    }
}

void LinkXTMAC::sendStrobe()
{
    if (outPkt == NULL && !transmitting && !ifsTimer->isScheduled() && nStrobe > 0 && strobePkt != NULL) {
        nStrobe--;
        outPkt = strobePkt->dup();
        notifyLower();
        // Set deadline
        scheduleAt(simTime() + par("strobeDeadline").doubleValue(), deadlineTimer);
        // Set timer for next period
        scheduleAt(simTime() + par("strobePeriod").doubleValue(), strobeTimer);
    }
    if (nStrobe <= 0 && strobePkt != NULL) {
        delete strobePkt;
        strobePkt = NULL;
    }
}

void LinkXTMAC::sendAck(macaddr_t addr)
{
    // Check for existed identical ack
    int i;
    Mac802154Pkt *pkt;
    for (i = 0; i < outQueue.getLength(); i++) {
        pkt = check_and_cast<Mac802154Pkt*>(outQueue.get(i));
        if (pkt->getPktType() == MAC802154_ACK && pkt->getDesAddr() == addr) {
            if (i == 0) prepareQueuedPkt(); // Send prepared ack at head of the queue
            return; // Duplicated
        }
    }

    Mac802154Pkt *ack = new Mac802154Pkt();
    ack->setPktType(MAC802154_ACK);
    ack->setSrcAddr(macAddr);
    ack->setDesAddr(addr);
    ack->setByteLength(ack->getPktSize());

    // Insert ACK at front of queue
    cPacket *front = outQueue.front();
    if (front == NULL) {
        outQueue.insert(ack);
    } else {
        outQueue.insertBefore(front, ack);
    }

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
    if (getId() == 2059) {
        printError(LV_ERROR, "Something wrong");
    }
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
    enableDutyCycling = true;
    mainPkt = NULL;
    strobePkt = NULL;
    nStrobe = 0;
    active = false;
    forcedActive = false;

    deadlineTimer = new cMessage("deadlineTimer");
    strobeTimer = new cMessage("strobeTimer");
    mainSendingTimer = new cMessage("mainSendingTimer");
    dcSleepTimer = new cMessage("dcSleepTimer");
    dcListenTimer = new cMessage("dcListenTimer");
    sendTimeout = new cMessage("sendTimeout");
}

LinkXTMAC::~LinkXTMAC()
{
    if (strobePkt != NULL) delete strobePkt;

    cancelAndDelete(deadlineTimer);
    cancelAndDelete(strobeTimer);
    cancelAndDelete(mainSendingTimer);
    cancelAndDelete(dcSleepTimer);
    cancelAndDelete(dcListenTimer);
    cancelAndDelete(sendTimeout);
}

}  // namespace twsn
