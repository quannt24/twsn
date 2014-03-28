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

namespace twsn {

Define_Module(LinkUnslottedCSMACA);

void LinkUnslottedCSMACA::initialize()
{
    BaseLink::initialized();
    aMaxBE = par("aMaxBE").longValue();
    macMaxNB = par("macMaxNB").longValue();
    macMinBE = par("macMinBE").longValue();
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
    }
}

void LinkUnslottedCSMACA::handleUpperMsg(cMessage* msg)
{
    /* Encapsulate packet from upper layer and start sending procedures. If it's not ready for
     * for sending (e.g. A packet is being sent), the new packet will be drop. */
    if (outPkt == NULL) {
        NetPkt *netpkt = check_and_cast<NetPkt*>(msg);
        MacPkt *macpkt = new MacPkt();

        // TODO Address resolution
        macpkt->setSrcAddr(netpkt->getSrcAddr());
        macpkt->setDesAddr(netpkt->getDesAddr());

        macpkt->setByteLength(macpkt->getByteLength());
        macpkt->encapsulate(netpkt);

        outPkt = macpkt; // Going to send this packet

        // Notify physical layer
        notifyLower();
    } else {
        printError(ERROR, "Not ready for sending");
    }
}

void LinkUnslottedCSMACA::handleUpperCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_NOTI:
            if (outPkt == NULL) fetchPacketFromUpper(); // TODO and fetchTimer is not scheduled
            delete cmd;
            break;

        default:
            // Just forward to lower layer
            if (cmd->getDes() != LINK)
                sendCtlDown(cmd);
            else
                delete cmd; // Unknown command
            break;
    }
}

void LinkUnslottedCSMACA::handleLowerMsg(cMessage* msg)
{
    MacPkt *macpkt = check_and_cast<MacPkt*>(msg);
    cPacket *netpkt = NULL;

    switch (macpkt->getPktType()) {
        case MAC_PKT_PAYLOAD:
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
                if (outPkt != NULL) {
                    sendDown(outPkt);
                    /* outPkt still holds value here only to mark that there is a packet being sent
                     * and it is not ready to send next packet. Do not use outPkt after this point,
                     * it will be set to NULL after receive CMD_READY from physical layer. */
                }
            } else {
                // Channel is busy, update variables and backoff again
                nextRound();
            }
            delete cmd;
            break;

        case CMD_READY:
        {
            // Reset outPkt pointer so that we can send next packet
            outPkt = NULL;
            // Fetch next packet from queue TODO After IFS
            fetchPacketFromUpper(); // TODO and fetchTimer is not scheduled
            // Switch radio transceiver to listen mode
            Command *rxcmd = new Command();
            rxcmd->setSrc(LINK);
            rxcmd->setDes(PHYS);
            rxcmd->setCmdId(CMD_PHY_RX);
            sendCtlDown(rxcmd);
            delete cmd;
        }
            break;

        default:
            // Just forward to upper layer
            if (cmd->getDes() != LINK) sendCtlUp(cmd);
            else delete cmd; // Unknown command
            break;
    }
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

void LinkUnslottedCSMACA::nextRound()
{
    nb++;
    be++;
    if (be > aMaxBE) be = aMaxBE;

    if (nb <= macMaxNB) {
        backoff();
    } else {
        // TODO Report failure
        EV << "LinkUnslottedCSMACA: Transmission failure\n";
    }
}

LinkUnslottedCSMACA::LinkUnslottedCSMACA()
{
    outPkt = NULL;
    backoffTimer = new cMessage("backoffTimer");
    listenTimer = new cMessage("listenTimer");
}

LinkUnslottedCSMACA::~LinkUnslottedCSMACA()
{
    cancelAndDelete(backoffTimer);
    cancelAndDelete(listenTimer);
}

}  // namespace twsn
