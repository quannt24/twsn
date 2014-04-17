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

#ifndef __TWSN_LINKUNSLOTTEDCSMACA_H_
#define __TWSN_LINKUNSLOTTEDCSMACA_H_

#include <omnetpp.h>
#include "baselink.h"
#include "mac802154pkt_m.h"

namespace twsn {

/**
 * Link layer with unslotted CSMA/CA
 */
class LinkUnslottedCSMACA : public BaseLink
{
    protected:
        cPacketQueue outQueue;
        Mac802154Pkt *outPkt; // Packet being sent
        bool transmitting; // Transmitting packet

        int aMaxBE;
        int macMaxNB;
        int macMinBE;
        int nb; // Number of current backoff
        int be; // Backoff exponent

        double ifsLen; // Length of interframe spacing (IFS)

        /* Timers */
        cMessage *backoffTimer; // End of backoff
        cMessage *listenTimer; // Start listen
        /* Timer for fetching packet from queue, this timer also for implementing IFS,
         * do not cancel it if you are not sure. */
        cMessage *ifsTimer;

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

        /**
         * Notify lower layer (physical) that there is a packet going to be sent.
         * Sending procedures will be started right after physical module ready (with
         * CMD_DATA_FETCH command arrive at lowerCtl gate).
         */
        void notifyLower();
        /** Start sending procedures to send outPkt */
        void startSending();
        /** Backoff with random backoff expenent */
        void backoff();
        /** Request physical layer to perform CCA for a specific duration */
        void performCCA();
        /** Send packet to physical layer, leaded by a command for switching to TX mode */
        void sendPkt();
        /** Defer sending of packet, update variables and begin next round */
        void deferPkt();
        /** Reset outPkt and switch to RX mode. Fetch next packet after IFS. */
        virtual void reset();

    public:
        LinkUnslottedCSMACA();
        ~LinkUnslottedCSMACA();
};

}  // namespace twsn

#endif
