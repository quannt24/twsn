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

#include "acousticsensor.h"
#include "sensemsg_m.h"
#include "basemobility.h"

namespace twsn {

Define_Module(AcousticSensor);

void AcousticSensor::startSense()
{
    cModule *wsn = getModuleByPath("^.^");
    int numTargets = wsn->par("numTargets");
    int i;
    cCompoundModule *tar;
    BaseMobility *mob1, *mob2;
    double d; // Distance from the sensor to target

    mob1 = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));
    // Clear old result
    meaList.clear();

    // Set timer to send result to application. If cannot sense any target, the meaList will be empty.
    scheduleAt(simTime() + par("responseDelay").doubleValue(), responseTimer);

    for (i = 0; i < numTargets; i++) {
        tar = check_and_cast<cCompoundModule*>(wsn->getSubmodule("target", i));
        mob2 = check_and_cast<BaseMobility*>(tar->getSubmodule("mobility"));
        d = distance(mob1->getCoord(), mob2->getCoord());

        // If the sensor is in range of one target, send request
        if (d <= tar->getSubmodule("generator")->par("ssRange").doubleValue()) {
            // Send request
            SenseAction *action = new SenseAction("SenseAction");
            action->setSensorId(this->getId());
            action->setMobilityId(getModuleByPath("^.mobility")->getId());
            sendDirect(action, tar->getSubmodule("generator"), "reqIn");
        }
    }
}

void AcousticSensor::processSignal(SenseSignal* ss)
{
    double md = ss->getDistance() + noise->nextNoise();; // Measured distance
    double ssRange = getModuleByPath("target[0].generator")->par("ssRange").doubleValue();
    if (md < 0) md = 0;
    if (md > ssRange) md = ssRange;

    Measurement mea;
    //mea.setTarId(ss->getTarId());
    mea.setMeasuredDistance(md);
    mea.setTrueDistance(ss->getDistance());
    mea.setTarCoord(Coord(ss->getX(), ss->getY()));
    // Coordinate of the node is set by application

    meaList.push_back(mea);
    delete ss;
}

void AcousticSensor::initialize()
{
    // Configure noise
    noise = new GaussianNoise(0,
            getModuleByPath("target[0].generator")->par("ssRange").doubleValue() * par("sigmaRate").doubleValue());
}

void AcousticSensor::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (msg == responseTimer) {
            // Send sensing result to application
            SenseResult *sr = new SenseResult();
            sr->setMsgType(SS_RESULT);
            sr->setMeaList(meaList);
            send(sr, "appGate$o");
        }
    } else {
        if (msg->getArrivalGate() == gate("appGate$i")) {
            SenseMsg *sm = check_and_cast<SenseMsg*>(msg);

            if (sm->getMsgType() == SS_START) {
                // Cancel current sensing action (if any) and start new sensing
                cancelEvent(responseTimer);
                startSense();
            } else if (sm->getMsgType() == SS_CANCEL) {
                cancelEvent(responseTimer);
            } else {
                printError(LV_ERROR, "Unknown message");
            }

            delete msg;
        } else if (msg->getArrivalGate() == gate("senseIn")) {
            SenseSignal *ss = check_and_cast<SenseSignal*>(msg);
            processSignal(ss);
        } else {
            printError(LV_ERROR, "Unknown message");
            delete msg;
        }
    }
}

AcousticSensor::AcousticSensor()
{
    noise = NULL;
    responseTimer = new cMessage("responseTimer");
}

AcousticSensor::~AcousticSensor()
{
    if (noise != NULL) delete noise;
    cancelAndDelete(responseTimer);
}

}  // namespace twsn
