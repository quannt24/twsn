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

#include "apptrackingsensor.h"
#include "apppkt_m.h"
#include "stathelper.h"

namespace twsn {

Define_Module(AppTrackingSensor);

void AppTrackingSensor::initFilterR()
{
    // TODO expand to multi-target
    fR = 0;
    fVr = 0;
}

void AppTrackingSensor::filterR(std::list<Measurement> &meaList)
{
    // TODO expand to multi-target
    double md = meaList.front().getMeasuredDistance(); // Measured distance
    double td = meaList.front().getTrueDistance();
    StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
    sh->recMeaError(md - td);

    double sensePeriod = par("sensePeriod").doubleValue();

    // Predict
    double pVr = fVr; // Predicted Vr
    double pR = fR + sensePeriod * fVr;

    // Update
    fVr = pVr + 0.25 * (md - pR) / sensePeriod;
    fR = pR + 0.75 * (md - pR);

    if (fR <= 0) {
        fR = 0;
    } else {
        // fR = 0 seem to be initial value, we ignore these estimated value
        sh->recEstError(fR - td);
    }
}

void AppTrackingSensor::initialize()
{
    BaseApp::initialized();

    // Start sensing, simulate unsynchronized sensing
    if (!senseTimer->isScheduled()) {
        scheduleAt(simTime() + uniform(0, par("sensePeriod").doubleValue()), senseTimer);
    }
}

void AppTrackingSensor::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGate() == gate("upper$i")) {
        handleUpperMsg(msg);
    } else if (msg->getArrivalGate() == gate("upperCtl$i")) {
        handleUpperCtl(msg);
    } else if (msg->getArrivalGate() == gate("lower$i")) {
        handleLowerMsg(msg);
    } else if (msg->getArrivalGate() == gate("lowerCtl$i")) {
        handleLowerCtl(msg);
    } else if (msg->getArrivalGate() == gate("ssGate$i")) {
        handleSenseMsg(check_and_cast<SenseMsg*>(msg));
    }
}

void AppTrackingSensor::handleSelfMsg(cMessage* msg)
{
    if (msg == senseTimer) {
        SenseMsg *ssStartMsg = new SenseMsg;
        ssStartMsg->setMsgType(SS_START);
        send(ssStartMsg, "ssGate$o");

        // Schedule next sensing
        scheduleAt(simTime() + par("sensePeriod").doubleValue(), senseTimer);
    }
}

void AppTrackingSensor::handleSenseMsg(SenseMsg* msg)
{
    SenseResult *sr = check_and_cast<SenseResult*>(msg);
    std::list<Measurement> meaList = sr->getMeaList();

    if (meaList.size() > 0) {
        filterR(meaList);
    } else {
        // No target sensed, reset (initialize) filter
        initFilterR();
    }

    delete msg;
}

AppTrackingSensor::AppTrackingSensor()
{
    initFilterR();
    senseTimer = new cMessage("SenseTimer");
}

AppTrackingSensor::~AppTrackingSensor()
{
    cancelAndDelete(senseTimer);
}

}  // namespace twsn
