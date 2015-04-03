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

package twsn.base.modules.linklayer;

import twsn.base.modules.ILayer;

//
// Interface for link layer
//
moduleinterface ILink extends ILayer
{
    parameters:
        // Address length in bytes (default is 8 for EUI-64)
        int addrLen;
        // Max size of a link layer frame in bytes, not including physical layer header
        int maxFrameSize;
        // Max packet size (in bytes) for upper layer
        int maxPacketSize;
        
        @display("i=block/layer");
}
