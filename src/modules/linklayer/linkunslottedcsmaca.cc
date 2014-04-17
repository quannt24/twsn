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

#include "linkunslottedcsmaca.h"
#include "netpkt_m.h"
#include "stathelper.h"

namespace twsn {

Define_Module(LinkUnslottedCSMACA);

void LinkUnslottedCSMACA::initialize()
{
    BaseLink::initialize();
    aMaxBE = par("aMaxBE").longValue();
    macMaxNB = par("macMaxNB").longValue();
    macMinBE = par("macMinBE").longValue();
    ifsLen = par("aMinLIFSPeriod").doubleValue();

    scheduleAt(0, listenTimer);
}

void LinkUnslottedCSMACA::handleSelfMsg(cMessage* msg)
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
        // Send next packet in queue
        if (!outQueue.empty()) {
            outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
            notifyLower();
        }
    }
}

void LinkUnslottedCSMACA::handleUpperMsg(cMessage* msg)
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

    // Send packet at head of the queue if ready
    if (!transmitting && !ifsTimer->isScheduled()) {
        outPkt = check_and_cast<Mac802154Pkt*>(outQueue.pop());
        notifyLower();
    }
}

void LinkUnslottedCSMACA::handleUpperCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    // Unknown command, just forward to lower layer
    if (cmd->getDes() != LINK) {
        sendCtlDown(cmd);
    } else {
        printError(WARNING, "Unknown command from upper");
        delete cmd; // Unknown command
    }
}

void LinkUnslottedCSMACA::handleLowerMsg(cMessage* msg)
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
            // Forward network packet to upper layer
            netpkt = macpkt->decapsulate();
            if (netpkt != NULL) {
                sendUp(netpkt);
            } else {
                printError(WARNING, "NULL payload");
            }
            delete macpkt;
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

void LinkUnslottedCSMACA::handleLowerCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_FETCH:
            startSending();
            delete cmd;
            break;

        case CMD_LIN_CCA_RESULT:
            if (((CmdCCAR*) cmd)->getClearChannel()) {
                // CCA success, send packet to physical layer
                sendPkt();
            } else {
                // Channel is busy, update variables and backoff again
                deferPkt();
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

void LinkUnslottedCSMACA::notifyLower()
{
    Command *cmd = new Command();
    cmd->setCmdId(CMD_DATA_NOTI);
    sendCtlDown(cmd);
}

void LinkUnslottedCSMACA::startSending()
{
    // Init CSMA/CA variables
    nb = 0;
    be = par("macMinBE").longValue();
    backoff();
}

void LinkUnslottedCSMACA::backoff()
{
    double backoffDur = intuniform(0, (int) (pow(2, be) - 1)) * par("aUnitBP").doubleValue();
    scheduleAt(simTime() + backoffDur, backoffTimer);
}

void LinkUnslottedCSMACA::performCCA()
{
    CmdCCA *cmd = new CmdCCA();
    cmd->setSrc(LINK);
    cmd->setDes(PHYS);
    cmd->setDuration(par("ccaDur").doubleValue());
    sendCtlDown(cmd);
}

void LinkUnslottedCSMACA::sendPkt()
{
    if (outPkt != NULL) {
        // Switch radio to TX mode
        Command *txcmd = new Command();
        txcmd->setSrc(LINK);
        txcmd->setDes(PHYS);
        txcmd->setCmdId(CMD_PHY_TX);
        sendCtlDown(txcmd);

        // Transmit
        sendDown(outPkt);
        outPkt = NULL;
        transmitting = true;

        // Prepare IFS
        if (outPkt->getByteLength() <= par("aMaxSIFSFrameSize").longValue()) {
            ifsLen = par("aMinSIFSPeriod").doubleValue();
        } else {
            ifsLen = par("aMinLIFSPeriod").doubleValue();
        }
    }
}

void LinkUnslottedCSMACA::deferPkt()
{
    nb++;
    be++;
    if (be > aMaxBE) be = aMaxBE;

    if (nb <= macMaxNB) {
        backoff();
    } else {
        getParentModule()->bubble("Tx failure");
        printError(INFO, "Tx failure");

        // Prepare IFS
        if (outPkt->getByteLength() <= par("aMaxSIFSFrameSize").longValue()) {
            ifsLen = par("aMinSIFSPeriod").doubleValue();
        } else {
            ifsLen = par("aMinLIFSPeriod").doubleValue();
        }

        delete outPkt;
        outPkt = NULL;
        reset();

        // Count packet loss
        StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
        sh->countLostMacPkt();
    }
}

void LinkUnslottedCSMACA::reset()
{
    // Reset transmitting flag so that we can send next packet
    transmitting = false;
    // Fetch next packet from queue after IFS
    if (!ifsTimer->isScheduled()) {
        scheduleAt(simTime() + ifsLen, ifsTimer);
    }
    // Switch radio transceiver to listen mode
    Command *rxcmd = new Command();
    rxcmd->setSrc(LINK);
    rxcmd->setDes(PHYS);
    rxcmd->setCmdId(CMD_PHY_RX);
    sendCtlDown(rxcmd);
}

LinkUnslottedCSMACA::LinkUnslottedCSMACA()
{
    outPkt = NULL;
    transmitting = false;
    backoffTimer = new cMessage("backoffTimer");
    listenTimer = new cMessage("listenTimer");
    ifsTimer = new cMessage("ifsTimer");
}

LinkUnslottedCSMACA::~LinkUnslottedCSMACA()
{
    cancelAndDelete(backoffTimer);
    cancelAndDelete(listenTimer);
    cancelAndDelete(ifsTimer);
}

}  // namespace twsn
