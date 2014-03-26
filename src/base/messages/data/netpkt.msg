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

cplusplus {{
	#include "twsndef.h"
}}
class noncobject netaddr_t;

//
// Base packet type for network layer
//
packet NetPkt {
    netaddr_t srcAddr; // Network address of sender
    netaddr_t desAddr; // Network address of sender or broadcast address
    bool preambleFlag = false; // Using one bit in flag field
    int hopLimit = 64;
    int pktSize = 18; // Packet size in bytes. Please use setByteLength() to set this size to packet.
}