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

package twsn.base.modules.phylayer;

import twsn.base.modules.BaseSimple;

//
// Base module for physical layer.
// In actual simulation, this module should not be used; extends it with more specific parameters.
//
simple BasePhy extends BaseSimple like IPhy
{
    parameters:
        @class("twsn::BasePhy");
		// Bit rate
        int bitRate @unit(bps) = default(250000 bps);

    gates:
        input upper @loose; // For data packets from the MAC layer
        input upperCtl @loose; // For control messages from the MAC layer        
}
