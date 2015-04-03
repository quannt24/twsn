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

package twsn.base.modules;

//
// Interface for a layer
//
moduleinterface ILayer
{
    @display("i=block/layer");

    gates:
        inout upper @loose; // For data packets from/to the upper layer
        inout upperCtl @loose; // For control messages from/to the upper layer
        inout lower; // For data packet from/to lower layer
        inout lowerCtl; // For control message from/to lower layer
}
