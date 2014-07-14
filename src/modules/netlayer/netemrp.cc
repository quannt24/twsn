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

#include "netemrp.h"
#include "baseenergy.h"
#include "basemobility.h"
#include "apppkt_m.h"
#include "decohelper.h"
#include "stathelper.h"

namespace twsn {

Define_Module(NetEMRP);

void NetEMRP::initialize()
{
    BaseNet::initialize();
    // Force link layer to stay active to initialize network
    scheduleAt(0, forceActiveTimer);

    if (!par("isBaseStation")) {
        // Set timer for initialize EMRP first time
        scheduleAt(uniform(0, par("initInterval").doubleValue()), reqRelayTimer);
    }
}

void NetEMRP::handleSelfMsg(cMessage* msg)
{
    if (msg == forceActiveTimer) {
        CmdForceActive *cfa = new CmdForceActive();
        cfa->setSrc(NETW);
        cfa->setDes(LINK);
        if (par("isBaseStation")) {
            cfa->setDuration(0); // Infinite
        } else {
            cfa->setDuration(par("initInterval").doubleValue()
                    + uniform(par("waitRelayInfoTimeout").doubleValue(), 2 * par("waitRelayInfoTimeout").doubleValue()));
        }
        sendCtlDown(cfa);
    } else if (msg == reqRelayTimer) {
        requestRelay();
    } else if (msg == waitRelayInfoTimer) {
        if (bsAddr != 0 || rnAddr != 0) {
            if (bsAddr != 0) {
                outPkt->setDesAddr(bsAddr);
            } else if (rnAddr != 0) {
                outPkt->setDesAddr(rnAddr);
            }
            sendDown(outPkt);
        } else {
            printError(LV_INFO, "Cannot find relay node. Dropping packet");
            if (outPkt->getPktType() == EMRP_PAYLOAD_TO_AN || outPkt->getPktType() == EMRP_PAYLOAD_TO_BS) {
                // Count lost packet
                StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
                sh->countLostNetPkt();
            }
            delete outPkt;
        }
        outPkt = NULL;
        // Fetch next packet in queue
        prepareQueuedPkt();
    } else if (msg == waitEnergyInfoTimer) {
        // Current relay node cannot report its energy information, replace it.
        rnAddr = 0;
        updateRelayEnergy(NULL);
    } else {
        // Self messages which are dynamically created (one time use)
        if (msg->getKind() == EMRP_TIMER_RES_RELAY) {
            netaddr_t des = check_and_cast<NetEmrpResRelayInfoTimer*>(msg)->getReqAddr();
            sendRelayInfo(des);
        }
        delete msg;
    }
}

void NetEMRP::handleUpperMsg(cMessage* msg)
{
    /* Encapsulate then enqueue packet from upper layer */
    AppPkt *apppkt = check_and_cast<AppPkt*>(msg);
    NetEmrpPkt *netpkt = new NetEmrpPkt();
    // Source address (this node address) will be set later by prepareQueuedPkt()

    switch (apppkt->getRoutingType()) {
        case RT_TO_BS:
            netpkt->setPktType(EMRP_PAYLOAD_TO_BS);
            // Destination address will be set later
            break;

        case RT_BROADCAST:
            netpkt->setPktType(EMRP_PAYLOAD_TO_AN);
            netpkt->setDesAddr(NET_BROADCAST_ADDR);
            break;

        default:
            // Default is unicast to an adjacent node
            netpkt->setPktType(EMRP_PAYLOAD_TO_AN);
            netpkt->setDesAddr(apppkt->getDesNetAddr());
            break;
    }

    // Set preamble flag
    if (netpkt->getPktType() == EMRP_PAYLOAD_TO_BS) {
        netpkt->setPreambleFlag(true);
    } else {
        netpkt->setPreambleFlag(false);
    }

    netpkt->setByteLength(netpkt->getPktSize());
    netpkt->encapsulate(apppkt);

    // Insert packet to queue
    outQueue.insert(netpkt);

    // Send packet at head of the queue if ready
    prepareQueuedPkt();
}

void NetEMRP::handleUpperCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    // Unknown command, just forward to lower layer
    if (cmd->getDes() != NETW) {
        sendCtlDown(cmd);
    } else {
        printError(LV_WARNING, "Unknown command from upper");
        delete cmd; // Unknown command
    }
}

void NetEMRP::handleLowerMsg(cMessage* msg)
{
    NetEmrpPkt *pkt = check_and_cast<NetEmrpPkt*>(msg);
    StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));

    // Check hop limit
    pkt->setHopLimit(pkt->getHopLimit() - 1);
    if (pkt->getHopLimit() <= 0) {
        printError(LV_INFO, "Hop limit exceeded. Dropping packet.");
        if (pkt->getPktType() == EMRP_PAYLOAD_TO_AN || pkt->getPktType() == EMRP_PAYLOAD_TO_BS) {
            // Count lost packet
            sh->countLostNetPkt();
        }
        delete pkt;
        return;
    }

    if (pkt->getPktType() == EMRP_PAYLOAD_TO_AN || pkt->getPktType() == EMRP_PAYLOAD_TO_BS) {
        // Count received packet
        sh->countRecvNetPkt();
    }

    // Process packet
    switch (pkt->getPktType()) {
        case EMRP_PAYLOAD_TO_AN:
            recvPayload(pkt);
            break;

        case EMRP_PAYLOAD_TO_BS:
            recvRelayedPayload(pkt);
            break;

        case EMRP_RELAY_REQ:
            recvRelayRequest(pkt);
            break;

        case EMRP_RELAY_INFO:
            recvRelayInfo(pkt);
            break;

        case EMRP_ENERGY_INFO:
            recvEnergyInfo(pkt);
            break;
    }
}

void NetEMRP::handleLowerCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    // Unknown command, just forward to upper layer
    if (cmd->getDes() != NETW) {
        sendCtlUp(cmd);
    } else {
        printError(LV_WARNING, "Unknown command from lower");
        delete cmd; // Unknown command
    }
}

void NetEMRP::prepareQueuedPkt()
{
    if (outPkt == NULL && !outQueue.empty()) {
        outPkt = check_and_cast<NetEmrpPkt*>(outQueue.pop());
        outPkt->setSrcAddr(macAddr);

        if (outPkt->getPktType() == EMRP_PAYLOAD_TO_BS) {
            if (bsAddr != 0 || rnAddr != 0) {
                if (bsAddr != 0) {
                    outPkt->setDesAddr(bsAddr);
                } else {
                    outPkt->setDesAddr(rnAddr);
                }
                // Send immediately
                sendDown(outPkt);
                outPkt = NULL;

                // Plan a timer for deadline of updating energy info
                cancelEvent(waitEnergyInfoTimer);
                scheduleAt(simTime() + par("waitEnergyInfoTimeout").doubleValue(), waitEnergyInfoTimer);

                prepareQueuedPkt(); // Be careful: recursive
            } else {
                // Find relay node
                requestRelay();
                // Wait for timeout to send
                scheduleAt(simTime() + par("waitRelayInfoTimeout").doubleValue(), waitRelayInfoTimer);
            }
        } else {
            // Send immediately
            sendDown(outPkt);
            outPkt = NULL;
            prepareQueuedPkt(); // Be careful: recursive
        }
    }
}

void NetEMRP::requestRelay()
{
    // Send request for relay node
    NetEmrpPkt *pkt = new NetEmrpPkt();
    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(NET_BROADCAST_ADDR);
    pkt->setPktType(EMRP_RELAY_REQ);
    pkt->setByteLength(pkt->getPktSize());

    sendDown(pkt);
}

void NetEMRP::sendRelayInfo(netaddr_t desAddr)
{
    BaseEnergy *ener = check_and_cast<BaseEnergy*>(getModuleByPath("^.energy"));
    BaseMobility *mob = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));
    NetEmrpRelayInfoPkt *pkt = new NetEmrpRelayInfoPkt();

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

void NetEMRP::sendEnergyInfo(netaddr_t desAddr)
{
    BaseEnergy *ener = check_and_cast<BaseEnergy*>(getModuleByPath("^.energy"));
    NetEmrpEnergyInfoPkt *pkt = new NetEmrpEnergyInfoPkt();

    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(desAddr);
    pkt->setPreambleFlag(false);
    pkt->setRemainEnergy(ener->getCapacity());

    pkt->setByteLength(pkt->getPktSize());
    sendDown(pkt);
}

bool NetEMRP::considerRelay(NetEmrpRelayInfoPkt *ri)
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
            // Current relay node becomes backup node
            bnAddr = rnAddr;
            enerBn = enerRn;
            dBsBn = dBsRn;
            dBn = dRn;

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

bool NetEMRP::considerBackup(NetEmrpRelayInfoPkt *ri)
{
    BaseMobility *mob = (BaseMobility*) getModuleByPath("^.mobility");
    // Distance from this node to candidate
    double dRc = distance(mob->getCoord(), Coord(ri->getPosX(), ri->getPosY()));

    if (bnAddr <= 0) {
        // No relay node has been selected, select this candidate
        bnAddr = ri->getSrcAddr();

        enerBn = ri->getEnergy();
        dBsBn = ri->getDBs();
        dBn = dRc;

        return true;
    } else {
        // Compare relay info of candidate to current relay node
        double a1 = assessRelay(enerBn, dBn, dBs, dBsBn);
        double a2 = assessRelay(ri->getEnergy(), dRc, dBs, ri->getDBs());

        if (a2 > a1) {
            bnAddr = ri->getSrcAddr();

            enerBn = ri->getEnergy();
            dBsBn = ri->getDBs();
            dBn = dRc;

            return true;
        } else {
            return false;
        }
    }
}

/*
 * Function for assessing a candidate for relaying.
 * Parameters:
 *  ener: energy of relay candidate
 *  dRc: distance from this node to relay candidate
 *  dBs: distance from this node to base station
 *  dRcBs: distance from relay candidate to base station
 */
double NetEMRP::assessRelay(double ener, double dRc, double dBs, double dRcBs)
{
    double cosa = (dRc*dRc + dBs*dBs - dRcBs*dRcBs) / (2 * dRcBs * dBs);
    return ener / dRcBs * cosa;
}

void NetEMRP::updateRelayEnergy(NetEmrpEnergyInfoPkt* eiPkt)
{
    if (bsAddr != 0) return; // No need to update
    if (eiPkt != NULL) {
        if (eiPkt->getSrcAddr() == rnAddr) {
            enerRn = eiPkt->getRemainEnergy(); // Update enerRn
        } else if (eiPkt->getSrcAddr() == bnAddr) {
            enerBn = eiPkt->getRemainEnergy(); // Update enerBn
        }
    }

    // Check critical energy value
    if (enerRn < par("criticalEnergy").doubleValue() || rnAddr <= 0) {
        if (enerBn < par("criticalEnergy").doubleValue() || bnAddr <= 0) {
            // Find new relay/backup nodes
            requestRelay();
        } else {
            // Switch to backup node
            switchRelayNode();
        }
        return;
    } else if (rnAddr > 0 && bnAddr > 0
            && enerBn - enerRn > par("switchingEnergy").doubleValue()) {
        switchRelayNode();
    }

    updateDecoration();
}

void NetEMRP::switchRelayNode()
{
    int tempi;
    double tempd;

    tempi = rnAddr;
    rnAddr = bnAddr;
    bnAddr = tempi;

    tempd = enerRn;
    enerRn = enerBn;
    enerBn = tempd;

    tempd = dBsRn;
    dBsRn = dBsBn;
    dBsBn = tempd;

    tempd = dRn;
    dRn = dBn;
    dBn = tempd;
}

void NetEMRP::recvPayload(NetEmrpPkt* pkt)
{
    AppPkt *apppkt = check_and_cast<AppPkt*>(pkt->decapsulate());
    sendUp(apppkt);
    delete pkt;
}

void NetEMRP::recvRelayedPayload(NetEmrpPkt* pkt)
{
    if (par("isBaseStation").boolValue()) {
        // Here is the destination, send packet to upper layer
        AppPkt *apppkt = check_and_cast<AppPkt*>(pkt->decapsulate());
        sendUp(apppkt);
        // Send back a report about energy (like a ACK)
        sendEnergyInfo(pkt->getSrcAddr());
        delete pkt;
    } else {
        if (bsAddr > 0 || (rnAddr > 0 && rnAddr != pkt->getSrcAddr())) {
            // Send back a report about energy (like a ACK)
            sendEnergyInfo(pkt->getSrcAddr());

            // Relay to next hop
            outQueue.insert(pkt);
            prepareQueuedPkt();
        } else if (bnAddr > 0 && bnAddr != pkt->getSrcAddr()) {
            // Change relay node to break loop
            rnAddr = 0;
            switchRelayNode();
            updateDecoration();

            // Send back a report about energy (like a ACK)
            sendEnergyInfo(pkt->getSrcAddr());

            // Relay to next hop
            outQueue.insert(pkt);
            prepareQueuedPkt();
        } else {
            // Deadend, cannot send now. We will drop the packet and repair routes.
            printError(LV_INFO, "Cannot relay packet, deadend!");
            if (pkt->getSrcAddr() == rnAddr) {
                // Refresh relay node (with hope to break loop)
                rnAddr = 0;
                updateRelayEnergy(NULL);
            }

            if (pkt->getPktType() == EMRP_PAYLOAD_TO_AN || pkt->getPktType() == EMRP_PAYLOAD_TO_BS) {
                // Count lost packet
                StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
                sh->countLostNetPkt();
            }
            delete pkt;
        }
    }
}

void NetEMRP::recvRelayRequest(NetEmrpPkt* pkt)
{
    NetEmrpResRelayInfoTimer *timer = new NetEmrpResRelayInfoTimer();
    timer->setReqAddr(pkt->getSrcAddr());
    timer->setKind(EMRP_TIMER_RES_RELAY);
    scheduleAt(simTime() + uniform(0, par("waitRelayInfoTimeout").doubleValue()), timer);

    delete pkt;
}

void NetEMRP::recvRelayInfo(NetEmrpPkt* pkt)
{
    NetEmrpRelayInfoPkt *ri = check_and_cast<NetEmrpRelayInfoPkt*>(pkt);

    if (ri->getBsFlag() == true) {
        bsAddr = ri->getSrcAddr();
        printError(LV_INFO, "Found new BS");
    } else {
        // No need to consider relay node if having connection with BS
        if (bsAddr <= 0) {
            bool ret = considerRelay(ri);
            if (ret) {
                printError(LV_INFO, "Found new relay node");
            } else {
                if (considerBackup(ri)) {
                    printError(LV_INFO, "Found new backup node");
                }
            }
        }
    }
    delete pkt;

    updateDecoration();
}

void NetEMRP::recvEnergyInfo(NetEmrpPkt* pkt)
{
    updateRelayEnergy(check_and_cast<NetEmrpEnergyInfoPkt*>(pkt));
    cancelEvent(waitEnergyInfoTimer);
    delete pkt;
}

void NetEMRP::updateDecoration()
{
    DecoHelper *dh = (DecoHelper*) getModuleByPath("decoHelper");
    if (dh == NULL) return;

    dh->disconnect(getParentModule());
    if (bsAddr > 0) {
        dh->dConnect(getParentModule(), simulation.getModule(bsAddr)->getParentModule(), "ls=#7777ff,1,s");
    } else if (rnAddr > 0) {
        dh->dConnect(getParentModule(), simulation.getModule(rnAddr)->getParentModule(), "ls=#9999ff,1,s");
    }
    if (bnAddr > 0 && bnAddr != rnAddr) {
        if (rnAddr == 0) {
            printError(LV_ERROR, "Missing relay node when backup node available");
        }
        dh->dConnect(getParentModule(), simulation.getModule(bnAddr)->getParentModule(), "ls=#ff9999,1,da");
    }
}

NetEMRP::NetEMRP()
{
    bsAddr = 0;
    rnAddr = 0;
    bnAddr = 0;

    dBs = 0;

    enerRn = 0;
    dBsRn = 0;
    enerBn = 0;
    dBsBn = 0;

    outPkt = NULL;

    reqRelayTimer = new cMessage("reqRelayTimer");
    waitRelayInfoTimer = new cMessage("waitRelayInfoTimer");
    waitEnergyInfoTimer = new cMessage("waitEnergyInfoTimer");
    forceActiveTimer = new cMessage("forceActiveTimer");
}

NetEMRP::~NetEMRP()
{
    cancelAndDelete(reqRelayTimer);
    cancelAndDelete(waitRelayInfoTimer);
    cancelAndDelete(waitEnergyInfoTimer);
    cancelAndDelete(forceActiveTimer);
}

}  // namespace twsn
