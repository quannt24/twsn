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

#include "linkbmac.h"
#include "netpkt_m.h"
#include "stathelper.h"
#include "base802154phy.h"

namespace twsn {

Define_Module(LinkBMAC);

void LinkBMAC::initialize()
{
    // Just copy LinkUnslottedCSMACA::initialize() without turning transceiver to RX mode
    BaseLink::initialize();
    aMaxBE = par("aMaxBE").longValue();
    macMaxNB = par("macMaxNB").longValue();
    macMinBE = par("macMinBE").longValue();
    ifsLen = par("aMinLIFSPeriod").doubleValue();

    // Plan initial check channel timer
    scheduleAt(par("checkInterval").doubleValue(), checkChannelTimer);
}

void LinkBMAC::handleSelfMsg(cMessage* msg)
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
    } else if (msg == checkChannelTimer) {
        switchToRx();
        // Wait aTurnAroundTime before performing CCA
        scheduleAt(simTime() + par("aTurnaroundTime").doubleValue(), waitTurnaroundTimer);
    } else if (msg == waitTurnaroundTimer) {
        performCCA();
    }
}

void LinkBMAC::handleUpperMsg(cMessage* msg)
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

    if (netpkt->getPreambleFlag()) {
        // Insert a preamble packet before the payload
        outQueue.insert(createPreamble());
    }

    outQueue.insert(macpkt);
    activate();

    // Send packet at head of the queue if ready
    prepareQueuedPkt();
}

void LinkBMAC::handleUpperCtl(cMessage* msg)
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

void LinkBMAC::handleLowerMsg(cMessage* msg)
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
            // Nothing to do
            delete macpkt;
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

void LinkBMAC::handleLowerCtl(cMessage* msg)
{
    // Copy from LinkUnslottedCSMACA and change behavior when receiving CCA result
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_FETCH:
            if (outPkt != NULL
                    && !backoffTimer->isScheduled()
                    && !ifsTimer->isScheduled()) {
                startSending();
            }
            delete cmd;
            break;

        case CMD_LIN_CCA_RESULT:
            if (((CmdCCAR*) cmd)->getClearChannel()) {
                if (outPkt != NULL) {
                    // CCA success, send packet to physical layer
                    sendPkt();
                    activate();
                } else {
                    // Come back to inactive state
                    deactive();
                }
            } else {
                if (outPkt != NULL) {
                    // Channel is busy, update variables and backoff again
                    deferPkt();
                }
                activate();
            }
            delete cmd;
            break;

        case CMD_READY:
            // Reset outPkt and switch to RX
            reset();
            delete cmd;
            break;

        default:
            // Unknown command, just forward to upper layer
            if (cmd->getDes() != LINK) {
                sendCtlUp(cmd);
            } else {
                printError(WARNING, "Unknown command from lower");
                delete cmd;
            }
            break;
    }
}

void LinkBMAC::activate(bool forced, double duration)
{
    // Switch radio transceiver to RX mode
    switchToRx();

    active = true;
    if (forced) {
        cancelEvent(checkChannelTimer);
        if (duration > 0) {
            printError(DEBUG, "Forced active");
            scheduleAt(simTime() + duration, checkChannelTimer);
        }
        forcedActive = true;
    } else {
        if (!forcedActive) {
            // If not in forcedActive interval then set dcSleepTimer as normal. If in forcedActive
            // interval, checkChannelTimer is already set and we will not change it. Otherwise, we
            // extend current checkChannelTimer a portion equals activeTime.
            cancelEvent(checkChannelTimer);
            scheduleAt(simTime() + par("activeTime").doubleValue(), checkChannelTimer);
        }
    }
}

void LinkBMAC::deactive()
{
    active = false;
    forcedActive = false;
    switchToIdle();
    // Plan a check channel timer
    cancelEvent(checkChannelTimer);
    scheduleAt(simTime() + par("checkInterval").doubleValue(), checkChannelTimer);
}

void LinkBMAC::prepareQueuedPkt()
{
    if (outPkt == NULL && !ifsTimer->isScheduled() && !outQueue.empty()) {
        outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
        notifyLower();
    }
}

Mac802154Pkt* LinkBMAC::createPreamble()
{
    Mac802154Pkt *pre = new Mac802154Pkt();

    pre->setPktType(MAC802154_PREAMBLE);
    pre->setSrcAddr(macAddr);
    pre->setDesAddr(MAC_BROADCAST_ADDR);
    pre->setByteLength(par("preambleLen").longValue());

    return pre;
}

void LinkBMAC::switchToRx()
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

void LinkBMAC::switchToIdle()
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

LinkBMAC::LinkBMAC()
{
    active = false;
    forcedActive = false;

    checkChannelTimer = new cMessage("checkChannelTimer");
    waitTurnaroundTimer = new cMessage("waitTurnaroundTimer");
}

LinkBMAC::~LinkBMAC()
{
    cancelAndDelete(checkChannelTimer);
    cancelAndDelete(waitTurnaroundTimer);
}

}  // namespace twsn
