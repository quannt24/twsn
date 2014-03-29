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

package twsn.base.modules.mobility;

import twsn.base.modules.BaseSimple;

//
// Base for all mobility modules
//
simple BaseMobility extends BaseSimple like IMobility
{
    parameters:
        @class("twsn::BaseMobility");

        bool moving = default(false); // Equal 'true' if object is moving

        // Parameters for static (non-moving) Entity
        double posX @unit(m) = default(0);
        double posY @unit(m) = default(0);

        // Parameters for moving Entity
        double startMovingTime @unit(s) = default(0); // Time for start moving (from start of simulation)
        bool stopSimWhenFinishMoving = default(false); // Stop simulation when finish moving
        
        @display("i=block/cogwheel");
}
