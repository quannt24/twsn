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

#include "acousticgenerator.h"
#include "sensemsg_m.h"
#include "basemobility.h"
#include "coord.h"

namespace twsn {

Define_Module(AcousticGenerator);

void AcousticGenerator::initialize()
{
    cDisplayString &ds = getParentModule()->getDisplayString();

    std::ostringstream oss;
    oss << par("ssRange").doubleValue() << '\0';

    ds.setTagArg("r", 0, oss.str().c_str());
}

void AcousticGenerator::handleMessage(cMessage *msg)
{
    SenseAction *sa = check_and_cast<SenseAction*>(msg);
    BaseMobility *tarMob = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));
    BaseMobility *ssMob = check_and_cast<BaseMobility*>(simulation.getModule(sa->getMobilityId()));

    // Create signal message and send back to sensor
    SenseSignal *sig = new SenseSignal();
    //sig->setTarId(getId());
    sig->setDistance(distance(tarMob->getCoord(), ssMob->getCoord())); // True distance
    sig->setX(tarMob->getCoordX());
    sig->setY(tarMob->getCoordY());

    sendDirect(sig, simulation.getModule(sa->getSensorId()), "senseIn");

    // Delete requesting message
    delete msg;
}

}  // namespace twsn
