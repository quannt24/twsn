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

#ifndef __TWSN_BASEWIRELESSPHY_H_
#define __TWSN_BASEWIRELESSPHY_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "basephy.h"
#include "channelmgr.h"
#include "airframe_m.h"
#include "macpkt_m.h"

namespace twsn {

/**
 * Base module for wireless physical layer
 */
class BaseWirelessPhy : public BasePhy
{
    protected:
        /* ChannelMgr object simulating wireless channel */
        ChannelMgr *channelMgr;
        /* PhyEntry holding channel information and connections of this node */
        PhyEntry* phyEntry;
        bool transmitting;

        /* Timer */
        cMessage *finishTxTimer;

        /** Override to use multiple initialization stages */
        virtual int numInitStages () const { return 2; };
        virtual void initialize();
        virtual void initialize(int stage);
        /** Register with ChannelMgr */
        virtual void registerChannel();

        /** Delegate jobs to other handling functions */
        virtual void handleMessage(cMessage *msg);
        /** Handle self message */
        virtual void handleSelfMsg(cMessage *msg);
        /** Handle message sent directly */
        virtual void handleAirFrame(AirFrame *frame);

        /** Transmit a MAC packet (broadcast or unicast) */
        virtual void txMacPkt(MacPkt *pkt);
        /** Finish transmission */
        virtual void finishTx();

        /**
         * Transmit air frame to the air (point to point). The frame must be created completely.
         * If channel registration has not completed (phyEntry == NULL), the frame will be deleted.
         */
        virtual void sendAirFrame(AirFrame *frame);
        /**
         * Called by handleAirFrame().
         * Receive air frame procedures when cooperating with ChannelMgr. If channel registration
         * has not completed (phyEntry == NULL), the frame will be deleted.
         */
        virtual void recvAirFrame(AirFrame *frame);

    public:
        BaseWirelessPhy();
        ~BaseWirelessPhy();
};

}

#endif
