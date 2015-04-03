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

package twsn.modules.mobility;

import twsn.base.modules.mobility.BaseMobility;

//
// Mobility module with predefined discrete moving path
//
simple MobilityPath extends BaseMobility
{
    parameters:
        @class("twsn::MobilityPath");
        
        // File that storing moving path
        // Each line of path file contain coordination of one point in form: "x y" (without ")
        string pathFile;
        double movingTimeStep @unit(s) = default(0.25 s); // Time for "jumping to next point" in the path
}
