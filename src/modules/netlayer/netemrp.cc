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
    }
}

void NetEMRP::handleUpperMsg(cMessage* msg)
{
    // TODO
}

void NetEMRP::handleUpperCtl(cMessage* msg)
{
    // TODO
}

void NetEMRP::handleLowerMsg(cMessage* msg)
{
    NetEmrpPkt *pkt = check_and_cast<NetEmrpPkt*>(msg);

    if (pkt->getPktType() == EMRP_RELAY_INFO) {
        //printError(VERBOSE, "Relay info received");

        NetEmrpRelayInfoPkt *ri = check_and_cast<NetEmrpRelayInfoPkt*>(pkt);
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
    }

    delete msg;
}

void NetEMRP::handleLowerCtl(cMessage* msg)
{
    // TODO
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

    bcRelayInfoTimer = new cMessage("reqRelayTimer");
}

NetEMRP::~NetEMRP()
{
    cancelAndDelete(bcRelayInfoTimer);
}

}  // namespace twsn
