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

namespace twsn {

Define_Module(StatHelper);

void StatHelper::pollTotalResEnergy()
{
    cModule *wsn = getModuleByPath("^");
    BaseEnergy *ener;
    int n = wsn->par("numNodes").longValue();
    double tre = 0;
    int i;

    for (i = 0; i < n; i++) {
        ener = check_and_cast<BaseEnergy*>(wsn->getSubmodule("node", i)->getSubmodule("energy"));
        tre += ener->getCapacity();
    }

    emit(sigTRE, tre);
}

void StatHelper::initialize()
{
    // Register signals
    sigMeaError = registerSignal("sigMeaError");
    sigPosError = registerSignal("sigPosError");
    sigTRE = registerSignal("sigTRE");

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

}  // namespace twsn
