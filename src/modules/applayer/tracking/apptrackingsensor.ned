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

package twsn.modules.applayer.tracking;

import twsn.base.modules.applayer.BaseApp;

//
// Application for tracking sensor node
//
simple AppTrackingSensor extends BaseApp
{
    parameters:
        @class("twsn::AppTrackingSensor");
        
        double sensePeriod @unit(s) = default(0.5 s);
        
        // Collect measurement interval (after receive measurement from sensor)
        double collMeaInterval @unit(s) = default(0.1 s);
        // Report measurement interval (should shorter than collMeaInterval
        double repMeaInterval @unit(s) = default(0.095 s);
        // Time point for CH broadcasts a synchronization beacon, relative to sensePeriod.
        // This point should be near the end of sensePeriod and long enough for beacon arrives
        // before next sensing cycle.
        double chBeaconTime @unit(s) = default(0.45 s);
        
        // Minimum number of measurement required to estimate target position
        int minNumMeasurement = default(3);

    gates:
        inout ssGate;
}
