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

#include "basemobility.h"

namespace twsn {

Define_Module(BaseMobility);

void BaseMobility::initialize()
{
    // Call initialize() of parent
    BaseSimple::initialize();

    setCoord(par("posX").doubleValue(), par("posY").doubleValue());
    updateDisplay();
}

/*
 * Update display of parent module in simulation
 */
void BaseMobility::updateDisplay()
{
    Enter_Method_Silent("updateDisplay");

    cDisplayString &ds = getParentModule()->getDisplayString();

    // Update displayed position according to object's coordination (x,y)
    ds.setTagArg("p", 0, coord.getX());
    ds.setTagArg("p", 1, coord.getY());
}

}
