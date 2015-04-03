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
	#include "apppkt_m.h"
	#include "measurement.h"
	#include "targetpos.h"
}}

namespace twsn;

packet AppPkt;
class noncobject Measurement;
class noncobject TargetPos;

// FOR REFERENCE
//enum RoutingType {
//    RT_TO_AN = 0; // Message to adjacent node
//    RT_TO_BS = 1; // Message to base station
//    RT_BROADCAST = 2; // Message to broadcast
//};

// FOR REFERENCE
// Base packet type for application layer
//
//packet AppPkt {
//    // Source address should only be set by network layer.
//    // Address fields here help simplifying simulation
//    netaddr_t srcNetAddr; // This field will not be count in packet size
//    netaddr_t desNetAddr; // This field will not be count in packet size
//    int routingType = RT_TO_AN; // Routing type, 1 byte
//    int pktSize = 1; // Packet size in bytes
//}

enum AppTrackingPktType {
    AT_SYNC_REQUEST = 0; // Nofity other nodes to synchronize sensing
    AT_MEASUREMENT = 1; // Raw measurements
    AT_TARGET_POSITION = 2; // Tracked postions of targets
    AT_CH_BEACON = 3; // CH's beacon for synchronizing sensing cycle
};

packet AppTrackingPkt extends AppPkt {
    pktSize = 2;
    int pktType;
}

packet AT_MeasurementPkt extends AppTrackingPkt {
    routingType = RT_BROADCAST;
    pktType = AT_MEASUREMENT;
    pktSize = 18;
    
    Measurement measurement;
}

packet AT_TargetPosPkt extends AppTrackingPkt {
    routingType = RT_TO_BS;
    pktType = AT_TARGET_POSITION;
    pktSize = 19;
    
    TargetPos targetPos;
}