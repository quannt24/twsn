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

package twsn.entities.nodes;

import twsn.modules.applayer.tracking.AppTrackingSensor;
import twsn.modules.other.AcousticSensor;

//
// Sensor node for tracking
//
module TrackingNode extends Node802154
{
    parameters:
        energyType = default("Battery");

        net.isBaseStation = false;

    submodules:
        app: AppTrackingSensor {
            @display("p=150,350");
        }
        sensor: AcousticSensor {
            @display("p=50,350");
        }
    connections:
        net.upper <--> app.lower;
        net.upperCtl <--> app.lowerCtl;
        app.ssGate <--> sensor.appGate;
}
