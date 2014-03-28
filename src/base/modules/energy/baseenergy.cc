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

#include "baseenergy.h"

namespace twsn {

Define_Module(BaseEnergy);

void BaseEnergy::initialize()
{
    BaseSimple::initialized();
    setCapacity(par("initCapacity").doubleValue());
}

double BaseEnergy::getCapacity()
{
    return capacity;
}

void BaseEnergy::setCapacity(double cap)
{
    Enter_Method("setCapacity");

    if (par("hasLinePower").boolValue()) {
        capacity = INFINITE_ENERGY;
    } else {
        if (cap >= 0) {
            capacity = cap;
        } else {
            capacity = 0;
        }
    }
}

double BaseEnergy::draw(double amount)
{
    Enter_Method("draw");

    if (par("hasLinePower").boolValue()) {
        return amount;
    } else {
        // Draw energy from battery
        double cap = getCapacity();

        if (amount > 0 && amount <= cap) {
            setCapacity(cap - amount);
            return amount;
        } else if (amount > cap) {
            setCapacity(0);
            //App *app = check_and_cast<App*>(getParentModule()->getSubmodule("app"));
            //if (app->isWorking()) app->poweroff(); // Check working state to prevent loop calls.
            return cap;
        } else {
            return 0;
        }
    }
}

}  // namespace twsn
