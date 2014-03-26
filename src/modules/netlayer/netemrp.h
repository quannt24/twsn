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

/**
 * Network layer using EMRP protocol
 */
class NetEMRP : public BaseNet
{
    protected:
        // MAC addresses for routing
        // Value 0 means connection info is not initialized.
        int macAddr; // MAC address of this node
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

        /* Timers */
        cMessage *bcRelayInfoTimer;

        /** Override to use multiple initialization stages */
        virtual int numInitStages() const { return 2; }
        virtual void initialize(int stage);
        virtual void initialize();

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

        /** Broadcast request for info of base station/relay/backup node */
        void requestRelay();
        /** Broadcast relay information. This method should only be called by a timer with a
         * random time point in an interval. */
        void broadcastRelayInfo();

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

    public:
        NetEMRP();
        ~NetEMRP();
};

}  // namespace twsn

#endif
