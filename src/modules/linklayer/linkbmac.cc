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

    enableDutyCycling = par("enableDutyCycling").boolValue();

    if (enableDutyCycling) {
        // Plan initial check channel timer
        gotoSleep();
    } else {
        // Switch transceiver to RX
        scheduleAt(0, listenTimer);
    }
}

void LinkBMAC::handleSelfMsg(cMessage* msg)
{
    if (msg == backoffTimer) {
        performCCA();
    } else if (msg == listenTimer) {
        // Switch radio transceiver to listen mode
        Command *cmd = new Command();
        cmd->setSrc(LINK);
        cmd->setDes(PHYS);
        cmd->setCmdId(CMD_PHY_RX);
        sendCtlDown(cmd);
    } else if (msg == ifsTimer) {
        // Send packet at head of the queue if ready
        prepareQueuedPkt();
    } else if (msg == deferNotiTimer) {
        notifyLower();
    } else if (msg == sleepTimer) {
        if (enableDutyCycling) gotoSleep(); // Go to sleep whatever forcedAwake or not
    } else if (msg == checkChannelTimer) {
        if (enableDutyCycling && outPkt == NULL && !transmitting) {
            Base802154Phy *phy = check_and_cast<Base802154Phy*>(getModuleByPath("^.phy"));
            if (phy->getRadioMode() == RX) {
                performCCA();
            } else {
                switchIdleToRx();
                // Wait aTurnAroundTime before performing CCA
                scheduleAt(simTime() + par("aTurnaroundTime").doubleValue(), deferCCATimer);
            }
        }
    } else if (msg == deferCCATimer) {
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

    if (!forcedAwake) {
        outQueue.insert(createPreamble());
    }

    outQueue.insert(macpkt);

    // Send packet at head of the queue if ready
    prepareQueuedPkt();
}

void LinkBMAC::handleUpperCtl(cMessage* msg)
{
    // Copy LinkUnslottedCSMACA::handleUpperCtl and add more functions
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_LIN_FORCE_ACTIVE:
            if (enableDutyCycling) {
                CmdForceActive *cfa = check_and_cast<CmdForceActive*>(cmd);
                wakeup(true, cfa->getDuration());
            }
            delete cmd;
            break;

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
            if (enableDutyCycling) {
                wakeup();
            }

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
            // Nothing to do
            delete macpkt;
            break;

        default:
            printError(LV_WARNING, "Unknown MAC packet type");
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
                } else if (enableDutyCycling && !forcedAwake) {
                    // Come back to inactive state
                    gotoSleep();
                }
            } else {
                if (outPkt != NULL) {
                    // Channel is busy, update variables and backoff again
                    deferPkt();
                }
                if (enableDutyCycling) wakeup();
            }
            delete cmd;
            break;

        case CMD_READY:
            // Reset outPkt and 'transmitting' flag
            reset();
            if (enableDutyCycling && !forcedAwake) {
                // Go to sleep after sending
                gotoSleep();
            } else {
                // Switch to RX if not duty cycling
                Command *rxcmd = new Command();
                rxcmd->setSrc(LINK);
                rxcmd->setDes(PHYS);
                rxcmd->setCmdId(CMD_PHY_RX);
                sendCtlDown(rxcmd);
            }
            delete cmd;
            break;

        default:
            // Unknown command, just forward to upper layer
            if (cmd->getDes() != LINK) {
                sendCtlUp(cmd);
            } else {
                printError(LV_WARNING, "Unknown command from lower");
                delete cmd;
            }
            break;
    }
}


void LinkBMAC::reset()
{
    // Reset outPkt pointer so that we can send next packet
    outPkt = NULL;
    transmitting = false;

    // Fetch next packet from queue after IFS
    cancelEvent(ifsTimer);
    scheduleAt(simTime() + ifsLen, ifsTimer);
}

void LinkBMAC::prepareQueuedPkt()
{
    if (outPkt == NULL && !transmitting && !ifsTimer->isScheduled()
            && !outQueue.empty() && ! deferNotiTimer->isScheduled()) {

        outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());

        Base802154Phy *phy = check_and_cast<Base802154Phy*>(getModuleByPath("^.phy"));
        if (phy->getRadioMode() == RX) {
            notifyLower();
        } else {
            switchIdleToRx();
            scheduleAt(simTime() + par("aTurnaroundTime").doubleValue(), deferNotiTimer);
        }
    }
}

Mac802154Pkt* LinkBMAC::createPreamble()
{
    Mac802154Pkt *pre = new Mac802154Pkt();

    pre->setPktType(MAC802154_PREAMBLE);
    pre->setSrcAddr(macAddr);
    pre->setDesAddr(MAC_BROADCAST_ADDR);
    pre->setPktSize(par("preambleLen").longValue());
    pre->setByteLength(pre->getPktSize());

    return pre;
}

void LinkBMAC::wakeup(bool forced, double duration)
{
    // Switch radio transceiver to RX mode (if currently in IDLE)
    switchIdleToRx();

    awake = true;
    cancelEvent(checkChannelTimer);
    cancelEvent(deferCCATimer);

    if (forced) {
        cancelEvent(sleepTimer);
        if (duration > 0) {
            printError(LV_DEBUG, "Forced active");
            scheduleAt(simTime() + duration, sleepTimer);
        }
        forcedAwake = true;
    } else {
        if (!forcedAwake) {
            // If in forcedAwake interval, sleepTimer is already set and we will not change it;
            // Otherwise, we extend (or set new) current sleepTimer a portion equals listenTimeout.
            cancelEvent(sleepTimer);
            scheduleAt(simTime() + par("listenTimeout").doubleValue(), sleepTimer);
        }
    }
}

void LinkBMAC::gotoSleep()
{
    awake = false;
    forcedAwake = false;
    switchToIdle();
    // Plan a check channel timer
    cancelEvent(checkChannelTimer);
    scheduleAt(simTime() + par("checkInterval").doubleValue(), checkChannelTimer);
}

void LinkBMAC::switchIdleToRx()
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
    awake = false;
    forcedAwake = false;

    deferNotiTimer = new cMessage("deferNotiTimer");
    checkChannelTimer = new cMessage("checkChannelTimer");
    deferCCATimer = new cMessage("deferCCATimer");
    sleepTimer = new cMessage("sleepTimer");
}

LinkBMAC::~LinkBMAC()
{
    cancelAndDelete(deferNotiTimer);
    cancelAndDelete(checkChannelTimer);
    cancelAndDelete(deferCCATimer);
    cancelAndDelete(sleepTimer);
}

}  // namespace twsn
