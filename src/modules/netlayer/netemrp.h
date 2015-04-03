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

#ifndef __TWSN_NETEMRP_H_
#define __TWSN_NETEMRP_H_

#include <omnetpp.h>
#include "basenet.h"
#include "coord.h"
#include "netemrppkt_m.h"

namespace twsn {

/** Dynamic created timer types */
enum EmrpTimerType {EMRP_TIMER_RES_RELAY};

/**
 * Network layer using EMRP protocol
 */
class NetEMRP : public BaseNet
{
    protected:
        // MAC addresses for routing
        // Value 0 means connection info is not initialized.
        int bsAddr;
        int rnAddr;
        int bnAddr;

        // Stored information about relay/backup nodes
        double enerRn; // Energy
        double dBsRn; // Distance from relay node to base station in meter
        double dRn; // Distance from relay node to this node
        double enerBn;
        double dBsBn;
        double dBn;

        cPacketQueue outQueue; // Sending queue for payload
        NetEmrpPkt *outPkt; // Packet going to be sent

        /* Timers */
        cMessage *forceActiveTimer; // Force link layer to stay active to initialize network
        cMessage *reqRelayTimer;
        cMessage *waitRelayInfoTimer;
        cMessage *waitEnergyInfoTimer;

        virtual void initialize(); // MAC address is obtained by BaseNet initializing functions

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
        /** Broadcast request for info of base station/relay/backup node */
        void requestRelay();
        /** Send relay information to specific node */
        void sendRelayInfo(netaddr_t desAddr);
        /** Send energy information to a node (unicast) */
        void sendEnergyInfo(netaddr_t desAddr);

        /**
         * Compare new relay info with current relay node; if it's better, select it as new relay node.
         * Return true when new the node having new relay info is selected as new relay node.
         */
        bool considerRelay(NetEmrpRelayInfoPkt *ri);
        /**
         * Compare new relay info with current backup node; if it's better, select it as new backup node.
         * Return true when new the node having new relay info is selected as new backup node.
         */
        bool considerBackup(NetEmrpRelayInfoPkt *ri);
        /**
         * Function for assessing a candidate for relaying.
         * Parameters:
         *  ener: energy of relay candidate
         *  dRc: distance from this node to relay candidate
         *  dBs: distance from this node to base station
         *  dRcBs: distance from relay candidate to base station
         */
        double assessRelay(double ener, double dRc, double dBs, double dRcBs);
        /**
         * Update energy of relay node when receive an energy reporting packet.
         * Perform switch relay node or find new relay node if necessary.
         * To discard current relay node and find new one (not guarantee new one is different from
         * old one) assign rnAddr = 0 then call this method with eiPkt = NULL.
         */
        void updateRelayEnergy(NetEmrpEnergyInfoPkt *eiPkt);
        /**
         * Switch between relay/backup nodes
         */
        void switchRelayNode();

        /**
         * Decapsulate and send payload to upper layer. Network header will be deleted.
         * This function is only for normal payload. Relayed payload should be handle by
         * recvRelayedPayload().
         */
        void recvPayload(NetEmrpPkt *pkt);
        /** Process relayed payload */
        void recvRelayedPayload(NetEmrpPkt *pkt);
        /** Handle relay request */
        void recvRelayRequest(NetEmrpPkt *pkt);
        /** Handle relay information */
        void recvRelayInfo(NetEmrpPkt *pkt);
        /** Handle updated energy information of relay node */
        void recvEnergyInfo(NetEmrpPkt *pkt);

        /** Update decoration */
        void updateDecoration();

    public:
        NetEMRP();
        ~NetEMRP();
};

}  // namespace twsn

#endif
