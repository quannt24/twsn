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

#include "netarpees.h"
#include "stathelper.h"
#include "apppkt_m.h"
#include "baseenergy.h"
#include "basemobility.h"
#include "downqueue.h"

namespace twsn {

Define_Module(NetARPEES);

void NetARPEES::handleSelfMsg(cMessage* msg)
{
    if (msg == waitRelayInfoTimer) {
        if (bsAddr != 0 || rnAddr != 0) {
            if (bsAddr != 0) {
                outPkt->setDesAddr(bsAddr);
            } else if (rnAddr != 0) {
                outPkt->setDesAddr(rnAddr);
            }
            sendDown(outPkt);
        } else {
            // TODO Report failure
            printError(INFO, "Cannot find relay node. Dropping packet");
            delete outPkt;
            // Count lost packet
            StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
            sh->countLostNetPkt();
        }
        outPkt = NULL;
        // Fetch next packet in queue
        fetchPacketFromUpper();
    } else {
        // Self messages which are dynamically created (one time use)
        if (msg->getKind() == ARPEES_TIMER_RES_RELAY) {
            netaddr_t des = check_and_cast<NetArpeesResRelayInfoTimer*>(msg)->getReqAddr();
            sendRelayInfo(des);
        }
        delete msg;
    }
}

void NetARPEES::handleUpperMsg(cMessage* msg)
{
    bool sendDelayed = false; // Delay sending to wait for relay information

    /* Encapsulate packet from upper layer and start sending procedures. */
    if (outPkt == NULL) {
        // Note: Use mac address for routing. Front end address is only set here (setSrcNetAddr).
        AppPkt *apppkt = check_and_cast<AppPkt*>(msg);
        apppkt->setSrcNetAddr(macAddr); // TODO Move to app layer

        NetArpeesPkt *netpkt = new NetArpeesPkt();
        netpkt->setSrcAddr(macAddr);

        switch (apppkt->getRoutingType()) {
            case RT_TO_BS:
                netpkt->setPktType(ARPEES_PAYLOAD_TO_BS);
                // Reset bsAddr and rnAddr, then request for new relay information
                bsAddr = 0;
                rnAddr = 0;
                sendDelayed = true;
                break;

            case RT_BROADCAST:
                netpkt->setPktType(ARPEES_PAYLOAD_TO_AN);
                netpkt->setDesAddr(NET_BROADCAST_ADDR);
                break;

            default:
                // Default is unicast to an adjacent node
                netpkt->setPktType(ARPEES_PAYLOAD_TO_AN);
                netpkt->setDesAddr(apppkt->getDesNetAddr());
                break;
        }

        // Set preamble flag
        if (netpkt->getPktType() == ARPEES_PAYLOAD_TO_BS) {
            netpkt->setPreambleFlag(true);
        } else {
            netpkt->setPreambleFlag(false);
        }

        netpkt->setByteLength(netpkt->getPktSize());
        netpkt->encapsulate(apppkt);
        outPkt = netpkt;

        if (!sendDelayed) {
            sendDown(outPkt);
            outPkt = NULL;
            fetchPacketFromUpper();
        } else {
            // Find relay node
            requestRelay();
            // Wait for timeout to retry
            scheduleAt(simTime() + par("waitRelayInfoTimeout").doubleValue(), waitRelayInfoTimer);
        }
    } else {
        printError(ERROR, "Not ready for sending. Dropping packet");
        delete msg;
        // Count lost packet
        StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
        sh->countLostNetPkt();
    }
}

void NetARPEES::handleUpperCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_NOTI:
            if (outPkt == NULL) fetchPacketFromUpper();
            delete cmd;
            break;

        default:
            // Just forward to lower layer
            if (cmd->getDes() != NETW)
                sendCtlDown(cmd);
            else
                printError(WARNING, "Unknown command");
                delete cmd; // Unknown command
            break;
    }
}

void NetARPEES::handleLowerMsg(cMessage* msg)
{
    NetArpeesPkt *pkt = check_and_cast<NetArpeesPkt*>(msg);
    StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));

    // Check hop limit
    pkt->setHopLimit(pkt->getHopLimit() - 1);
    if (pkt->getHopLimit() <= 0) {
        printError(INFO, "Hop limit exceeded. Dropping packet.");
        delete pkt;
        // Count lost packet
        sh->countLostNetPkt();
        return;
    }

    // Count received packet
    sh->countRecvNetPkt();

    // Process packet
    switch (pkt->getPktType()) {
        case ARPEES_PAYLOAD_TO_AN:
            recvPayload(pkt);
            break;

        case ARPEES_PAYLOAD_TO_BS:
            recvRelayedPayload(pkt);
            break;

        case ARPEES_RELAY_REQ:
            recvRelayRequest(pkt);
            break;

        case ARPEES_RELAY_INFO:
            recvRelayInfo(pkt);
            break;
    }
}

void NetARPEES::handleLowerCtl(cMessage* msg)
{
    // todo
}

void NetARPEES::requestRelay()
{
    // Send request for relay node
    NetArpeesPkt *pkt = new NetArpeesPkt();
    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(NET_BROADCAST_ADDR);
    pkt->setPktType(ARPEES_RELAY_REQ);
    pkt->setByteLength(pkt->getPktSize());

    sendDown(pkt);
}

void NetARPEES::sendRelayInfo(netaddr_t desAddr)
{
    BaseEnergy *ener = check_and_cast<BaseEnergy*>(getModuleByPath("^.energy"));
    BaseMobility *mob = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));
    NetArpeesRelayInfoPkt *pkt = new NetArpeesRelayInfoPkt();

    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(desAddr);
    pkt->setBsFlag(par("isBaseStation").boolValue());
    pkt->setEnergy(ener->getCapacity());
    pkt->setPosX(mob->getCoordX());
    pkt->setPosY(mob->getCoordY());
    pkt->setDBs(dBs); // Every node knows position of BS thank to helper

    pkt->setByteLength(pkt->getPktSize());
    sendDown(pkt);
}

void NetARPEES::recvPayload(NetArpeesPkt* pkt)
{
    AppPkt *apppkt = check_and_cast<AppPkt*>(pkt->decapsulate());
    sendUp(apppkt);
    delete pkt;
}

void NetARPEES::recvRelayedPayload(NetArpeesPkt* pkt)
{
    AppPkt *apppkt = check_and_cast<AppPkt*>(pkt->decapsulate());
    if (par("isBaseStation").boolValue()) {
        // Here is the destination, send packet to upper layer
        sendUp(apppkt);
    } else {
        // Enqueue packet
        apppkt->setKind(DownQueue::ENQUEUE);
        sendUp(apppkt);
    }
    delete pkt;
}

void NetARPEES::recvRelayRequest(NetArpeesPkt* pkt)
{
    NetArpeesResRelayInfoTimer *timer = new NetArpeesResRelayInfoTimer();
    timer->setReqAddr(pkt->getSrcAddr());
    timer->setKind(ARPEES_TIMER_RES_RELAY);
    scheduleAt(simTime() + uniform(0, par("waitRelayInfoTimeout").doubleValue()), timer);

    delete pkt;
}

void NetARPEES::recvRelayInfo(NetArpeesPkt* pkt)
{
    NetArpeesRelayInfoPkt *ri = check_and_cast<NetArpeesRelayInfoPkt*>(pkt);

    if (ri->getBsFlag() == true) {
        bsAddr = ri->getSrcAddr();
        printError(INFO, "Found new BS");
    } else {
        // No need to consider relay node if having connection with BS
        if (bsAddr <= 0) {
            if (considerRelay(ri)) {
                printError(INFO, "Found new relay node");
            }
        }
    }
    delete pkt;
}

bool NetARPEES::considerRelay(NetArpeesRelayInfoPkt* ri)
{
    BaseMobility *mob = (BaseMobility*) getModuleByPath("^.mobility");
    // Distance from this node to candidate
    double dRc = distance(mob->getCoord(), Coord(ri->getPosX(), ri->getPosY()));

    if (rnAddr <= 0) {
        // No relay node has been selected, select this candidate
        rnAddr = ri->getSrcAddr();
        enerRn = ri->getEnergy();
        dBsRn = ri->getDBs();
        dRn = dRc;
        return true;
    } else {
        // Compare relay info of candidate to current relay node
        double a1 = assessRelay(enerRn, dRn, dBs, dBsRn);
        double a2 = assessRelay(ri->getEnergy(), dRc, dBs, ri->getDBs());

        if (a2 > a1) {
            // Store information of new relay node
            rnAddr = ri->getSrcAddr();
            enerRn = ri->getEnergy();
            dBsRn = ri->getDBs();
            dRn = dRc;
            return true;
        } else {
            return false;
        }
    }
}

double NetARPEES::assessRelay(double ener, double dRc, double dBs, double dRcBs)
{
    double cosa = (dRc * dRc + dBs * dBs - dRcBs * dRcBs) / (2 * dRcBs * dBs);
    return ener / dRcBs * cosa;
}

NetARPEES::NetARPEES()
{
    bsAddr = 0;
    rnAddr = 0;

    outPkt = NULL;

    waitRelayInfoTimer = new cMessage("waitRelayInfoTimer");
}

NetARPEES::~NetARPEES()
{
    cancelAndDelete(waitRelayInfoTimer);
}

}  // namespace twsn
