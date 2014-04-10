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

package twsn.base.modules.energy;

import twsn.base.modules.BaseSimple;

//
// Base module for energy. Default is line power with infinite capacity.
//
simple BaseEnergy extends BaseSimple like IEnergy
{
    parameters:
        @class("twsn::BaseEnergy");
        
        // If module having line power source (infinite)
        bool hasLinePower = default(true);
        // Initial capacity, use -1 to represent infinite capacity (e.g. line power)
        double initCapacity @unit(mWh) = default(-1 mWh);
        
        // Residual energy
        @signal[sigResEnergy](type="double");
        @statistic[residualEnergy](title="Residual energy"; source="sigResEnergy"; record=last);
        
        @display("i=block/plug");
}
