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
#include "basewirelessphy.h"

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
    LinkUnslottedCSMACA::handleSelfMsg(msg);

    if (msg == strobeTimer) {
        if (nStrobe > 0) {
            // Send next strobe
            sendStrobe();
        } else {
            std::cerr << "Finish sending strobes t=" << simTime() << endl;
            delete strobePkt;
            strobePkt = NULL;

            // Send main packet anyway
            outPkt = mainPkt;
            // Notify physical layer
            notifyLower();
        }
    } else if (msg == deadlineTimer) {
        if (nStrobe > 0 && strobePkt != NULL) {
            // Cancel current being sent strobe
            printError(INFO, "Strobe deadline is missed");
            nStrobe--;
            if (!outPkt->isScheduled()) {
                delete outPkt;
            }
            outPkt = NULL;
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
            }
        } else {
            printError(WARNING, "NULL main packet, fetch for other");
            fetchPacketFromUpper();
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

    /* Encapsulate packet from upper layer and start sending procedures. If it's not ready for
     * for sending (e.g. A packet is being sent), the new packet will be drop. */
    if (mainPkt == NULL && outPkt == NULL) {
        NetPkt *netpkt = check_and_cast<NetPkt*>(msg);
        Mac802154Pkt *macpkt = new Mac802154Pkt();

        // TODO Address resolution
        macpkt->setSrcAddr(netpkt->getSrcAddr());
        macpkt->setDesAddr(netpkt->getDesAddr());

        macpkt->setByteLength(macpkt->getPktSize());
        macpkt->encapsulate(netpkt);

        mainPkt = macpkt; // Going to send this packet
        activate();

        if (netpkt->getPreambleFlag()) {
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
            // Notify physical layer
            notifyLower();
        }
    } else {
        printError(ERROR, "Not ready for sending");
        delete msg;
        // Count packet loss
        StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
        sh->countLostMacPkt();
    }
}

void LinkXTMAC::handleUpperCtl(cMessage* msg)
{
    // Copy LinkUnslottedCSMACA::handleUpperCtl and add more functions
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_NOTI:
            if (mainPkt == NULL && outPkt == NULL && !fetchTimer->isScheduled()) {
                // Fetch timer also help bypass continuous notification
                scheduleAt(simTime(), fetchTimer);
            }
            delete cmd;
            break;

        case CMD_LIN_FORCE_ACTIVE: {
            CmdForceActive *cfa = check_and_cast<CmdForceActive*>(cmd);
            activate(true, cfa->getDuration());
            delete cmd;
            break;
        }

        default:
            // Just forward to lower layer
            if (cmd->getDes() != LINK)
                sendCtlDown(cmd);
            else
                printError(WARNING, "Unknown command from upper");
                delete cmd; // Unknown command
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
        delete msg;
        // Count packet loss
        sh->countLostMacPkt();
        return;
    }

    // Count received packet
    sh->countRecvMacPkt();

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
            }
            delete macpkt;
            break;

        case MAC802154_ACK:
            printError(INFO, "ACK received");
            delete macpkt;
            activate();

            // Cancel strobing
            nStrobe = 0;
            delete strobePkt;
            strobePkt = NULL;
            cancelEvent(strobeTimer);
            cancelEvent(deadlineTimer);
            if (outPkt != NULL && outPkt->getPktType() == MAC802154_PREAMBLE) {
                cancelEvent(backoffTimer);
                delete outPkt;
                outPkt = NULL;
            }
            // Send main packet (after an interval equal IFS)
            scheduleAt(simTime() + ifsLen, mainSendingTimer);
            break;

        default:
            printError(WARNING, "Unknown MAC packet type");
            delete macpkt;
            // Count packet loss
            StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
            sh->countLostMacPkt();
            break;
    }
}

void LinkXTMAC::reset()
{
    if (outPkt == mainPkt) {
        // Main packet sent. Fetch next packet from queue after IFS.
        if (!fetchTimer->isScheduled()) {
            scheduleAt(simTime() + ifsLen, fetchTimer);
        }
        // Reset mainPkt pointer so that we can send next packet
        mainPkt = NULL;
    } else {
        // Strobe sent
        nStrobe--;
        cancelEvent(deadlineTimer);
    }

    // Reset outPkt pointer
    outPkt = NULL;

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

void LinkXTMAC::sendStrobe()
{
    if (outPkt == NULL && nStrobe > 0 && strobePkt != NULL) {
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
    if (outPkt == NULL) {
        Mac802154Pkt *ack = new Mac802154Pkt();
        ack->setPktType(MAC802154_ACK);
        ack->setSrcAddr(macAddr);
        ack->setDesAddr(addr);
        ack->setByteLength(ack->getPktSize());

        outPkt = ack;
        notifyLower();
    }
}

void LinkXTMAC::switchToRx()
{
    BaseWirelessPhy *phy = check_and_cast<BaseWirelessPhy*>(getModuleByPath("^.phy"));
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
    BaseWirelessPhy *phy = check_and_cast<BaseWirelessPhy*>(getModuleByPath("^.phy"));
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
