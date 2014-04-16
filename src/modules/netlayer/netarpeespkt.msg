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
	#include "netpkt_m.h"
}}

namespace twsn;

packet NetPkt;
class noncobject netaddr_t;

// FOR REFERENCE
//packet NetPkt {
//    netaddr_t srcAddr; // Network address of sender
//    netaddr_t desAddr; // Network address of sender or broadcast address
//    bool preambleFlag = false; // Using one bit in flag field
//    int hopLimit = 32;
//    int pktSize = 18; // Packet size in bytes. Please use setByteLength() to set this size to packet.
//}

enum ArpeesPktType {
    ARPEES_PAYLOAD_TO_AN = 0; // Packet to be transmited to adjacent node (cluster head or a specific node)
    ARPEES_PAYLOAD_TO_BS = 1; // Packet to be transmited to base station
    ARPEES_RELAY_REQ = 2; // Request for relay info
    ARPEES_RELAY_INFO = 3; // Relay info to response the request
};

// General ARPEES packet
packet NetArpeesPkt extends NetPkt {
    pktSize = 18;
    int pktType = ARPEES_PAYLOAD_TO_AN; // 3 bits in flag field
}

// ARPEES relay info packet
packet NetArpeesRelayInfoPkt extends NetArpeesPkt {
    pktSize = 34;
    pktType = ARPEES_RELAY_INFO;
    
    bool bsFlag = false; // Flag on if sender is base station, using 1 bit in flag field
    double energy = 0; // Current energy, 4 bytes
    double posX = 0; // Coordination x in meter, 4 bytes
    double posY = 0; // Coordination y in meter, 4 bytes
    double dBs = 0; // Distance to base station, 4 bytes
}

// Timer for sending back relay information to requesting node
message NetArpeesResRelayInfoTimer {
    netaddr_t reqAddr; // Address of the node which requesting
}