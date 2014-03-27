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
#include "basemobility.h"
#include "apppkt_m.h"

namespace twsn {

Define_Module(NetEMRP);

void NetEMRP::initialize(int stage)
{
    switch (stage) {
        case 0:
            initialize();
            break;

        case 1:
            macAddr = getModuleByPath("^.phy")->getId();
            /*char msg[100];
            sprintf(msg, "MAC address %d", (int) macAddr);
            printError(VERBOSE, msg);*/
            break;

        default:
            break;
    }
}

void NetEMRP::initialize()
{
    BaseNet::initialize();
    // Set timer for initialize EMRP first time
    scheduleAt(uniform(0, par("initInterval").doubleValue()), bcRelayInfoTimer);
}

void NetEMRP::handleSelfMsg(cMessage* msg)
{
    if (msg == bcRelayInfoTimer) {
        broadcastRelayInfo();
    } else if (msg == waitRelayInfoTimer) {
        if (bsAddr != 0 || rnAddr != 0) {
            sendDown(outPkt);
        } else {
            // TODO Report failure
            printError(INFO, "Cannot find relay node. Dropping packet");
            delete outPkt;
        }
        outPkt = NULL;
    } else if (msg == waitEnergyInfoTimer) {
        // Current relay node cannot report its energy information, replace it.
        rnAddr = 0;
        updateRelayEnergy(NULL);
    }
}

void NetEMRP::handleUpperMsg(cMessage* msg)
{
    bool sendDelayed = false; // Delay sending to wait for relay information

    /* Encapsulate packet from upper layer and start sending procedures. */
    if (outPkt == NULL) {
        // Note: Use mac address for routing. Front end address is only set here (setSrcNetAddr).
        AppPkt *apppkt = check_and_cast<AppPkt*>(msg);
        apppkt->setSrcNetAddr(macAddr);

        NetEmrpPkt *netpkt = new NetEmrpPkt();
        netpkt->setSrcAddr(macAddr);

        switch (apppkt->getRoutingType()) {
            case RT_TO_BS:
                netpkt->setPktType(EMRP_PAYLOAD_TO_BS);
                if (bsAddr != 0) {
                    netpkt->setDesAddr(bsAddr);
                } else if (rnAddr != 0) {
                    netpkt->setDesAddr(rnAddr);
                } else {
                    sendDelayed = true;
                }
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
        outPkt = netpkt;

        if (!sendDelayed) {
            sendDown(outPkt);
            outPkt = NULL;
        } else {
            // Find relay node
            requestRelay();
            // Wait for timeout to retry
            scheduleAt(simTime() + par("waitRelayInfoTimeout").doubleValue(), waitRelayInfoTimer);
        }
    } else {
        printError(ERROR, "Not ready for sending. Dropping packet");
        delete msg;
    }
}

void NetEMRP::handleUpperCtl(cMessage* msg)
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
                delete cmd; // Unknown command
            break;
    }
}

void NetEMRP::handleLowerMsg(cMessage* msg)
{
    NetEmrpPkt *pkt = check_and_cast<NetEmrpPkt*>(msg);

    // Check hop limit
    pkt->setHopLimit(pkt->getHopLimit() - 1);
    if (pkt->getHopLimit() <= 0) {
        if (pkt->getPktType() == EMRP_PAYLOAD_TO_BS && !par("isBaseStation").boolValue()) {
            printError(INFO, "Hop limit exceeded. Dropping packet.");
            delete pkt;
            return;
        }
    }

    // Process packet
    AppPkt *apppkt = NULL;
    NetEmrpRelayInfoPkt *ri = NULL;
    switch (pkt->getPktType()) {
        case EMRP_PAYLOAD_TO_AN:
            // Forward to upper layer
            apppkt = check_and_cast<AppPkt*>(pkt->decapsulate());
            sendUp(apppkt);
            delete pkt;
            break;

        case EMRP_PAYLOAD_TO_BS:
            if (par("isBaseStation").boolValue()) {
                // Here is the destination, send packet to upper layer
                AppPkt *apppkt = check_and_cast<AppPkt*>(pkt->decapsulate());
                sendUp(apppkt);
                // Send back a report about energy (like a ACK)
                sendEnergyInfo(pkt->getSrcAddr());
                delete pkt;
            } else {
                // Relay packet
                if (bsAddr <= 0 && (rnAddr <= 0 || pkt->getSrcAddr() == rnAddr)) {
                    // Here is a deadend, we should prevent loop here
                    printError(INFO, "Cannot relay packet, deadend!");
                    if (pkt->getSrcAddr() == rnAddr) {
                        // Refresh relay node (with hope to break loop)
                        rnAddr = 0;
                        updateRelayEnergy(NULL);
                    }
                    delete pkt;
                } else {
                    // Send back a report about energy (like a ACK)
                    sendEnergyInfo(pkt->getSrcAddr());

                    // Relay to next hop
                    pkt->setSrcAddr(macAddr);
                    if (bsAddr > 0) {
                        pkt->setPreambleFlag(false); // No need to send preamble
                        pkt->setDesAddr(bsAddr);
                    } else {
                        pkt->setDesAddr(rnAddr);
                    }
                    sendDown(pkt);

                    // Plan a timer for deadline of updating energy info
                    if (!waitEnergyInfoTimer->isScheduled()) {
                        scheduleAt(simTime() + par("waitEnergyInfoTimeout").doubleValue(), waitEnergyInfoTimer);
                    }
                }
            }
            break;

        case EMRP_REQ_RELAY:
            if (!bcRelayInfoTimer->isScheduled()) {
                // Broadcast relay information at a random time point in an interval equal
                // 'waitRelayInfoTimeout'
                scheduleAt(simTime() + uniform(0, par("waitRelayInfoTimeout").doubleValue()),
                        bcRelayInfoTimer);
            }
            delete pkt;
            break;

        case EMRP_RELAY_INFO:
            //printError(VERBOSE, "Relay info received");
            ri = check_and_cast<NetEmrpRelayInfoPkt*>(pkt);
            if (ri->getBsFlag() == true) {
                bsAddr = ri->getSrcAddr();
                printError(INFO, "Found new BS");
            } else {
                // No need to consider relay node if having connection with BS
                if (bsAddr <= 0) {
                    bool ret = considerRelay(ri);
                    if (ret) {
                        printError(INFO, "Found new relay node");
                    } else {
                        if (considerBackup(ri)) {
                            printError(INFO, "Found new backup node");
                        }
                    }
                }
            }
            delete pkt;
            break;

        case EMRP_ENERGY_INFO:
            updateRelayEnergy(check_and_cast<NetEmrpEnergyInfoPkt*>(pkt));
            cancelEvent(waitEnergyInfoTimer);
            delete pkt;
            break;
    }
}

void NetEMRP::handleLowerCtl(cMessage* msg)
{
    delete msg;
}

void NetEMRP::requestRelay()
{
    // Send request for relay node
    NetEmrpPkt *pkt = new NetEmrpPkt();
    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(NET_BROADCAST_ADDR);
    pkt->setPktType(EMRP_REQ_RELAY);
    pkt->setByteLength(pkt->getPktSize());

    sendDown(pkt);
}

void NetEMRP::broadcastRelayInfo()
{
    NetEmrpRelayInfoPkt *pkt = new NetEmrpRelayInfoPkt();
    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(NET_BROADCAST_ADDR);
    pkt->setBsFlag(par("isBaseStation").boolValue());
    // TODO Set energy, pos, dBS
    pkt->setByteLength(pkt->getPktSize());

    sendDown(pkt);
}

void NetEMRP::sendEnergyInfo(netaddr_t desAddr)
{
    NetEmrpEnergyInfoPkt *pkt = new NetEmrpEnergyInfoPkt();
    pkt->setSrcAddr(macAddr);
    pkt->setDesAddr(desAddr);
    pkt->setPreambleFlag(false);
    // TODO Set remaining energy
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
    if (eiPkt != NULL) enerRn = eiPkt->getRemainEnergy();

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
    } else if (enerBn - enerRn > par("switchingEnergy").doubleValue()) {
        switchRelayNode();
    }
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

    bcRelayInfoTimer = new cMessage("reqRelayTimer");
    waitRelayInfoTimer = new cMessage("waitRelayInfoTimer");
    waitEnergyInfoTimer = new cMessage("waitEnergyInfoTimer");
}

NetEMRP::~NetEMRP()
{
    cancelAndDelete(bcRelayInfoTimer);
    cancelAndDelete(waitRelayInfoTimer);
    cancelAndDelete(waitEnergyInfoTimer);
}

}  // namespace twsn
