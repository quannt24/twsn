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
// Network layer using ARPEES protocol
//
simple NetARPEES extends BaseNet
{
    parameters:
        @class("twsn::NetARPEES");
        
        // Timeout for waiting relay information after sending a relay request
        double waitRelayInfoTimeout @unit(s) = default(0.1 s);
}
