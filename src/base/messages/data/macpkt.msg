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

namespace twsn;

class noncobject macaddr_t;

//
// Base packet type for MAC layer
//
packet MacPkt {
    macaddr_t srcAddr; // MAC address of sender
    macaddr_t desAddr; // MAC address of sender or broadcast address
    int pktSize = 16; // Packet size in bytes
}