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
	#include "macpkt_m.h"
}}

namespace twsn;

packet MacPkt;

// FOR REFERENCE
// General MAC frame format		Octets
// ========================================
// Frame control				2
// Sequence number				1
// ----------------------------------------
// Addressing fields
// Destination PAN Identifier	0/2
// Destination Address			0/2/8
// Source PAN Identifier		0/2
// Source Address				0/2/8
// ----------------------------------------
// Auxiliary Security Header	0/5/6/10/14
// MAC Payload					variable
// FCS							2

// FOR REFERENCE
//packet MacPkt {
//    macaddr_t srcAddr; // MAC address of sender
//    macaddr_t desAddr; // MAC address of sender or broadcast address
//    int pktSize = 16; // Packet size in bytes
//}

enum Mac802154PktType {
    MAC802154_BEACON = 0; // 000: Beacon
    MAC802154_DATA = 1; // 001: Payload data
    MAC802154_ACK = 2; // 010: Acknowledgement
    MAC802154_CMD = 3; // 011: MAC Command
    // 100:111 are reserved. However, we add other types here for convenience.
    MAC802154_PREAMBLE = 4;
};

//
// A simplified IEEE 802.15.4 complaint MAC packet format
//
packet Mac802154Pkt extends MacPkt {
	int pktType = MAC802154_DATA; // Accounted in first 3 bits of "Frame control"
    pktSize = 21; // Without network coordinators, we use full addresses (8 bytes)
}