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

#ifndef __TWSN_NETARPEES_H_
#define __TWSN_NETARPEES_H_

#include <omnetpp.h>
#include "basenet.h"
#include "netarpeespkt_m.h"

namespace twsn {

/**
 * Network layer using ARPEES protocol
 */
class NetARPEES : public BaseNet
{
    public:
        /** Dynamic created timer types */
        enum ArpeesTimerType {ARPEES_TIMER_RES_RELAY};

    protected:
        // MAC addresses for routing
        // Value 0 means connection info is not initialized.
        int bsAddr; // Base station
        int rnAddr; // Relay node

        // Stored information about relay node
        double enerRn; // Energy
        double dBsRn; // Distance from relay node to base station in meter
        double dRn; // Distance from relay node to this node

        cPacketQueue outQueue;
        NetArpeesPkt *outPkt; // Packet going to be sent

        cMessage *waitRelayInfoTimer;

        /** Handle self message */
        virtual void handleSelfMsg(cMessage *msg);
        /** Handle message/packet from upper$i */
        virtual void handleUpperMsg(cMessage *msg);
        /** Handle control message from upperCtl$i */
        virtual void handleUpperCtl(cMessage *msg);
        /** Handle message/packet from lower$i */
        virtual void handleLowerMsg(cMessage *msg);
        /** Handle control message from lowerCtl$i */
        virtual void handleLowerCtl(cMessage *msg);

        /**
         * Pop packet from queue and prepare sending if not currently in sending procedures.
         */
        void prepareQueuedPkt();
        /** Broadcast request for info of base station/relay node */
        void requestRelay();
        /** Send relay information to specific node */
        void sendRelayInfo(netaddr_t desAddr);

        /**
         * Decapsulate and send payload to upper layer. Network header will be deleted.
         * This function is only for normal payload. Relayed payload should be handle by
         * recvRelayedPayload().
         */
        void recvPayload(NetArpeesPkt *pkt);
        /** Process relayed payload */
        void recvRelayedPayload(NetArpeesPkt *pkt);
        /** Handle relay request */
        void recvRelayRequest(NetArpeesPkt *pkt);
        /** Handle relay information */
        void recvRelayInfo(NetArpeesPkt *pkt);

        /**
         * Compare new relay info with current relay node; if it's better, select it as new relay node.
         * Return true when new the node having new relay info is selected as new relay node.
         */
        bool considerRelay(NetArpeesRelayInfoPkt *ri);
        /**
         * Function for assessing a candidate for relaying.
         * Parameters:
         *  ener: energy of relay candidate
         *  dRc: distance from this node to relay candidate
         *  dBs: distance from this node to base station
         *  dRcBs: distance from relay candidate to base station
         */
        double assessRelay(double ener, double dRc, double dBs, double dRcBs);

    public:
        NetARPEES();
        ~NetARPEES();
};

}  // namespace twsn
#endif
