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

package twsn.modules.netlayer;

import twsn.base.modules.netlayer.BaseNet;

//
// Network layer using EMRP protocol
//
simple NetEMRP extends BaseNet
{
    parameters:
        @class("twsn::NetEMRP");
        
        // Interval for initializing network at start of simulation.
        double initInterval @unit(s) = default(10 s);
        // Timeout for waiting relay information after sending a relay request
        double waitRelayInfoTimeout @unit(s) = default(0.2 s);
        // Timeout for Waiting for packet of energy update of relay node.
        // If updated info is not received before timeout, relay node will be changed (considered
        // as incident happended).
        double waitEnergyInfoTimeout @unit(s) = default(0.5 s);
        // Differential amount of relay/backup nodes' energy that triggers switch nodes
        double switchingEnergy @unit(mWh) = default(5 mWh);
        double criticalEnergy @unit(mWh) = default(0.1 mWh);
}
