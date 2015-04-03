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

#include "stathelper.h"
#include "baseenergy.h"
#include "basemobility.h"
#include <fstream>

namespace twsn {

Define_Module(StatHelper);

void StatHelper::pollTotalResEnergy()
{
    cModule *wsn = getModuleByPath("^");
    BaseEnergy *ener;
    int n = wsn->par("numNodes").longValue();
    double tre = 0; // Total residual energy
    int i;

    for (i = 0; i < n; i++) {
        ener = check_and_cast<BaseEnergy*>(wsn->getSubmodule("node", i)->getSubmodule("energy"));
        tre += ener->getCapacity();
    }

    emit(sigTRE, tre);
}

void StatHelper::recResEnergy()
{
    cConfigurationEx *configEx = ev.getConfigEx();
    std::ostringstream ossEner;
    std::ofstream outEner;

    cModule *wsn = getModuleByPath("^");
    BaseEnergy *ener;
    BaseMobility *mob;
    int n = wsn->par("numNodes").longValue();
    int i;

    ossEner.seekp(0);
    ossEner << "results/";
    ossEner << configEx->getActiveConfigName() << "_resEner.data\0";
    outEner.open(ossEner.str().c_str(), std::ios::out | std::ios::trunc);

    if (outEner) {
        outEner << "# Config: " << configEx->getActiveConfigName() << endl;
        outEner << "# Residual energy of each node" << endl;
        outEner << "# row col energy" << endl;

        for (i = 0; i < n; i++) {
            ener = check_and_cast<BaseEnergy*>(wsn->getSubmodule("node", i)->getSubmodule("energy"));
            mob = check_and_cast<BaseMobility*>(wsn->getSubmodule("node", i)->getSubmodule("mobility"));
            outEner << mob->getRow() << ' ' << mob->getCol() << ' ' << ener->getCapacity() << endl;
        }
    } else {
        std::cerr << "Cannot open file " << ossEner.str() << endl;
    }
    outEner.close();
}

void StatHelper::initialize()
{
    // Register signals
    sigMeaError = registerSignal("sigMeaError");
    sigPosError = registerSignal("sigPosError");
    sigTRE = registerSignal("sigTRE");
    sigRHD = registerSignal("sigRHD");
    sigRecvMacPkt = registerSignal("sigRecvMacPkt");
    sigLostMacPkt = registerSignal("sigLostMacPkt");
    sigRecvNetPkt = registerSignal("sigRecvNetPkt");
    sigLostNetPkt = registerSignal("sigLostNetPkt");
    sigCreatedRelayPkt = registerSignal("sigCreatedRelayPkt");
    sigDeliveredRelayPkt = registerSignal("sigDeliveredRelayPkt");

    // Start polling for total residual energy
    scheduleAt(0, pollTRETimer);
}

void StatHelper::handleMessage(cMessage* msg)
{
    if (msg == pollTRETimer) {
        pollTotalResEnergy();
        scheduleAt(simTime() + par("pollTREPeriod").doubleValue(), pollTRETimer);
    } else {
        delete msg;
    }
}

void StatHelper::finish()
{
    recResEnergy();
}

StatHelper::StatHelper()
{
    pollTRETimer = new cMessage("pollTRETimer");
}

StatHelper::~StatHelper()
{
    cancelAndDelete(pollTRETimer);
}

void StatHelper::recMeaError(double err)
{
    Enter_Method_Silent("recMeaError");
    emit(sigMeaError, err);
}

void StatHelper::recPosError(double err)
{
    Enter_Method_Silent("recPosError");
    emit(sigPosError, err);
}

void StatHelper::recRelayHopDelay(double delay)
{
    Enter_Method_Silent("recPosError");
    emit(sigRHD, delay);
}

void StatHelper::countRecvMacPkt()
{
    Enter_Method_Silent("countRecvMacPkt");
    emit(sigRecvMacPkt, 1);
}

void StatHelper::countLostMacPkt()
{
    Enter_Method_Silent("countLostMacPkt");
    emit(sigLostMacPkt, 1);
}

void StatHelper::countRecvNetPkt()
{
    Enter_Method_Silent("countRecvNetPkt");
    emit(sigRecvNetPkt, 1);
}

void StatHelper::countLostNetPkt()
{
    Enter_Method_Silent("countLostNetPkt");
    emit(sigLostNetPkt, 1);
}

void StatHelper::countCreatedRelayPkt()
{
    Enter_Method_Silent("countCreatedRelayPkt");
    emit(sigCreatedRelayPkt, 1);
}

void StatHelper::countDeliveredRelayPkt()
{
    Enter_Method_Silent("countDeliveredRelayPkt");
    emit(sigDeliveredRelayPkt, 1);
}

}  // namespace twsn
