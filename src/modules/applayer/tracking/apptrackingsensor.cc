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
#include "apptrackingpkt_m.h"
#include "stathelper.h"
#include "basemobility.h"
#include "baseenergy.h"
#include "matrix.h"

namespace twsn {

Define_Module(AppTrackingSensor);

void AppTrackingSensor::requestSyncSense()
{
    AppTrackingPkt *pkt = new AppTrackingPkt();
    pkt->setPktType(AT_SYNC_REQUEST);
    pkt->setRoutingType(RT_BROADCAST);
    pkt->setByteLength(pkt->getPktSize());
    sendDown(pkt);
}

void AppTrackingSensor::broadcastMeasurement()
{
    AT_MeasurementPkt *pkt = new AT_MeasurementPkt();
    pkt->setMeasurement(measurement);
    pkt->setByteLength(pkt->getPktSize());
    sendDown(pkt);
}

void AppTrackingSensor::promoteCH()
{
    if (!hasMeasurement) return; // This node could not sense target signal

    // Check if has enough measurement for tracking
    if ((unsigned) meaCollection.size() + 1 < par("minNumMeasurement").longValue()) return;

    // Promote this node to CH
    double myCHvalue = measurement.getNodeEnergy() / measurement.getMeasuredDistance();
    bool isCH = true;
    for (std::list<Measurement>::iterator it = meaCollection.begin(); it != meaCollection.end(); it++) {
        if (myCHvalue < (*it).getNodeEnergy() / (*it).getMeasuredDistance()) {
            isCH = false;
            break;
        }
    }

    if (isCH) {
        // Add measurement of this node to collection
        meaCollection.push_front(measurement);
        // Estimate target position
        TargetPos *tp = estimatePosition(meaCollection);

        if (tp != NULL) {
            getParentModule()->bubble("Target tracked");

            // todo Bound for valid estimation
            double ssRange = getModuleByPath("target[0].generator")->par("ssRange").doubleValue();
            if (distance(tp->getCoord(), mobility->getCoord()) > ssRange) {
                // Set target position as node position when having invalid estimation
                tp->setCoord(mobility->getCoord());
            }

            // Record true coordination for reference
            tp->setTrueCoord(measurement.getTarCoord());
            // Punch time stamp
            tp->setTimestamp(senseTimestamp);

            // Record positioning error
            StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
            sh->recPosError(distance(tp->getCoord(), measurement.getTarCoord()));

            // Send result to BS
            AT_TargetPosPkt *tpPkt = new AT_TargetPosPkt();
            tpPkt->setTargetPos(*tp);
            sendDown(tpPkt);

            delete tp;
        }

        // CH plans a timer for synchronizing sensing cycle in cluster
        cancelEvent(chBeaconTimer);
        scheduleAt(senseTimer->getArrivalTime()
                        - par("sensePeriod").doubleValue()
                        + par("chBeaconTime").doubleValue(), chBeaconTimer);
    }
}

TargetPos* AppTrackingSensor::estimatePosition(std::list<Measurement> &meaList)
{
    if ((unsigned) meaList.size() < par("minNumMeasurement").longValue()) return NULL;

    Matrix A = Matrix(meaList.size() - 1, 2);
    Matrix B = Matrix(meaList.size() - 1, 1);
    Matrix AA = Matrix(meaList.size() - 1, meaList.size() - 1);
    Measurement &m0 = meaList.front();
    double x0 = m0.getNodeCoord().getX();
    double y0 = m0.getNodeCoord().getY();
    double r0 = m0.getMeasuredDistance();
    double xi, yi, ri;

    int i = 0;
    std::list<Measurement>::iterator it = meaList.begin();
    it++; // Skip first element which is m0
    for (; it != meaList.end(); ++it) {
        xi = (*it).getNodeCoord().getX();
        yi = (*it).getNodeCoord().getY();
        ri = (*it).getMeasuredDistance();

        //set value of Matrix A
        A.setCell(i, 0, 2 * (xi - x0));
        A.setCell(i, 1, 2 * (yi - y0));

        //set value of Matrix B
        B.setCell(i, 0, (r0 * r0 - ri * ri) - (x0 * x0 - xi * xi) - (y0 * y0 - yi * yi));

        i++; // Next row
    }

    AA = A.transpose() * A;
    if (AA.getDet() != 0) {
        Matrix tarPosMat(2, 1);
        tarPosMat = AA.inverse() * A.transpose() * B;
        TargetPos *tarPos = new TargetPos();
        tarPos->setCoord(Coord(tarPosMat.getCell(0, 0), tarPosMat.getCell(1, 0)));
        return tarPos;
    } else {
        return NULL;
    }
}

void AppTrackingSensor::initialize()
{
    BaseApp::initialized();
    mobility = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));

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
        SenseMsg *smStart = new SenseMsg;
        smStart->setMsgType(SS_START);
        send(smStart, "ssGate$o");

        // Schedule next sensing
        scheduleAt(simTime() + par("sensePeriod").doubleValue(), senseTimer);
    } else if (msg == reportTimer) {
        broadcastMeasurement();
    } else if (msg == collTimer) {
        promoteCH();
    } else if (msg == chBeaconTimer) {
        // Broadcast a beacon
        AppTrackingPkt *beacon = new AppTrackingPkt();
        beacon->setRoutingType(RT_BROADCAST);
        beacon->setPktType(AT_CH_BEACON);
        beacon->setByteLength(beacon->getPktSize());
        sendDown(beacon);
        syncSense = true;
    }
}

void AppTrackingSensor::handleLowerMsg(cMessage* msg)
{
    AppTrackingPkt *pkt = check_and_cast<AppTrackingPkt*>(msg);

    if (pkt->getPktType() == AT_SYNC_REQUEST) {
        // Synchronize sensing cycle
        syncSense = true;

        // Cancel current sensing action (if any)
        SenseMsg *smCancel = new SenseMsg();
        smCancel->setMsgType(SS_CANCEL);
        send(smCancel, "ssGate$o");

        // Reset sensing timer with synchronized value
        // Adjusted time is calculated based on: sensePeriod, senseDelay and estimated txTime
        double addTime = par("sensePeriod").doubleValue()
                - getModuleByPath("^.sensor")->par("responseDelay").doubleValue(); // TODO -txTime
        cancelEvent(senseTimer);
        scheduleAt(simTime() + addTime, senseTimer);
    } else if (pkt->getPktType() == AT_MEASUREMENT) {
        // Collect sense results if in collMeaInterval
        if (collTimer->isScheduled()) {
            // Add measurement to collection
            AT_MeasurementPkt *atPkt = check_and_cast<AT_MeasurementPkt*>(pkt);
            Measurement m = atPkt->getMeasurement();
            meaCollection.push_back(m);
        }
    } else if (pkt->getPktType() == AT_CH_BEACON) {
        // Synchronize with CH
        syncSense = true;

        // If in sense delay period, cancel the sensing
        SenseMsg *smCancel = new SenseMsg();
        smCancel->setMsgType(SS_CANCEL);
        send(smCancel, "ssGate$o");

        // Cancel unfinished tasks (if any)
        cancelEvent(reportTimer);
        cancelEvent(collTimer);

        // Reset sensing timer with synchronized value
        // Adjusted time is calculated based on: sensePeriod, chBeaconTime and estimated txTime
        double addTime = par("sensePeriod").doubleValue()
                - par("chBeaconTime").doubleValue(); // TODO -txTime
        cancelEvent(senseTimer);
        scheduleAt(simTime() + addTime, senseTimer);
    }

    delete msg;
}

void AppTrackingSensor::handleSenseMsg(SenseMsg* msg)
{
    SenseResult *sr = check_and_cast<SenseResult*>(msg);
    std::list<Measurement> meaList = sr->getMeaList();

    if (meaList.size() > 0) {
        // Because we assume that sensor cannot separate signal from multiple targets, we only use smallest measured distance
        Measurement bestMea = meaList.front();
        for (std::list<Measurement>::iterator it = meaList.begin(); it != meaList.end(); it++) {
            if (bestMea.getMeasuredDistance() > (*it).getMeasuredDistance()) {
                bestMea = *it;
            }
        }
        this->measurement = bestMea;
        hasMeasurement = true;
        senseTimestamp = simTime().dbl();

        // Statistic distance measurement error
        StatHelper *sh = check_and_cast<StatHelper*>(getModuleByPath("statHelper"));
        sh->recMeaError(this->measurement.getMeasuredDistance() - this->measurement.getTrueDistance());

        if (!syncSense) {
            requestSyncSense();
            syncSense = true;
        } else {
            /* Add node's information to measurement object */
            BaseEnergy *ener = check_and_cast<BaseEnergy*>(getModuleByPath("^.energy"));
            measurement.setNodeCoord(mobility->getCoord());
            measurement.setNodeEnergy(ener->getCapacity());

            // Clear measurement of previous round
            meaCollection.clear();

            // Set timer for broadcasting measurement
            scheduleAt(simTime() + uniform(0, par("repMeaInterval").doubleValue()), reportTimer);
            // Set collect measurement timer
            scheduleAt(simTime() + par("collMeaInterval").doubleValue(), collTimer);
        }
    } else {
        // Come back to unsynchronized state
        syncSense = false;
        hasMeasurement = false;
    }

    delete msg;
}

AppTrackingSensor::AppTrackingSensor()
{
    mobility = NULL;

    syncSense = false;
    hasMeasurement = false;

    senseTimer = new cMessage("SenseTimer");
    reportTimer = new cMessage("ReportTimer");
    collTimer = new cMessage("CollTimer");
    chBeaconTimer = new cMessage("CHBeaconTimer");
}

AppTrackingSensor::~AppTrackingSensor()
{
    cancelAndDelete(senseTimer);
    cancelAndDelete(reportTimer);
    cancelAndDelete(collTimer);
    cancelAndDelete(chBeaconTimer);
}

}  // namespace twsn
