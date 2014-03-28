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
#include "command_m.h"

namespace twsn {

enum RadioMode {POWER_DOWN = 0, IDLE = 1, RX = 2, TX = 3};

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

        int radioMode; // Current radio mode
        simtime_t pcTimestamp; // Power consumption time stamp

        /* Timer */
        cMessage *finishTxTimer; // Simulate end of transmission
        cMessage *fetchTimer; // Timer for send CMD_DATA_FETCH to upper layer
        cMessage *switchTxTimer; // Simulate delay to switch to TX mode
        cMessage *switchRxTimer; // Simulate delay to switch to RX mode
        cMessage *switchIdleTimer; // Simulate delay to switch to IDLE mode
        cMessage *pcTimer; // Power consumption timer (simulate continuous draw)
        cMessage *ccaTimer; // Simulating CCA delay

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
        /** Handle message/packet from upper$i */
        virtual void handleUpperMsg(cMessage *msg);
        /** Handle control message from upperCtl$i */
        virtual void handleUpperCtl(cMessage *msg);
        /** Handle message sent directly */
        virtual void handleAirFrame(AirFrame *frame);

        /** Send data packet up */
        void sendUp(cPacket *pkt);
        /** Send control message up */
        void sendCtlUp(Command *cmd);

        /** Perform CCA, return result after CCA delay. In simulation, this function simply set
         * a timer for calling senseChannel() after delay time. */
        virtual void performCCA(double duration);
        /** Check channelState of phyEntry object. Report result to upper layer by a command. */
        virtual void senseChannel();

        /** Send command CMD_DATA_FETCH to fetch data packet from upper layer */
        virtual void fetchPacket();

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

        /** Start switching to a mode. It will take time for the transceiver switch between modes */
        virtual void switchRadioMode(int mode);
        /** Set radio mode. This function has effect immediately */
        virtual void setRadioMode(int mode);

        /** Calculate using time (based on pcTimestamp) and draw energy */
        virtual void draw();

    public:
        BaseWirelessPhy();
        ~BaseWirelessPhy();
};

}

#endif
