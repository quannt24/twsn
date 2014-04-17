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

#ifndef __TWSN_LINKXTMAC_H_
#define __TWSN_LINKXTMAC_H_

#include <omnetpp.h>
#include "linkunslottedcsmaca.h"

namespace twsn {

/**
 * Link layer with XT-MAC protocol
 */
class LinkXTMAC : public LinkUnslottedCSMACA
{
    protected:
        Mac802154Pkt *mainPkt; // Main packet being sent
        Mac802154Pkt *strobePkt;
        bool active;
        bool forcedActive;
        int nStrobe;

        cMessage *deadlineTimer;
        cMessage *strobeTimer;
        cMessage *mainSendingTimer; // Timer for sending main packet after strobes
        cMessage *dcSleepTimer;
        cMessage *dcListenTimer;

        virtual void initialize();
        /** Handle self message */
        virtual void handleSelfMsg(cMessage *msg);
        /** Handle message/packet from upper$i */
        virtual void handleUpperMsg(cMessage *msg);
        /** Handle control message from upperCtl$i */
        virtual void handleUpperCtl(cMessage *msg);
        /** Handle message/packet from lower$i */
        virtual void handleLowerMsg(cMessage *msg);

        /** Reset outPkt and switch to RX mode. Fetch next packet after IFS. */
        void reset();

        /**
         * Change to active state. If it is a forcedly activation (forced = true), layer will keep
         * active state for an interval specified by 'duration' before come back to normal duty
         * cycling. Passing a non-positive value to 'duration' when forced = true will keep active
         * state forever.
         */
        void activate(bool forced = false, double duration = 0);

        /**
         * Pop packet from queue and prepare sending if not currently in sending procedures.
         */
        void prepareQueuedPkt();
        void sendStrobe();
        /** Send and ACK to a specific address */
        void sendAck(macaddr_t addr);

        /** Request physical layer to switch to RX mode if currently in IDLE */
        void switchToRx();
        /** Request physical layer to switch to IDLE mode */
        void switchToIdle();

    public:
        LinkXTMAC();
        ~LinkXTMAC();
};

}  // namespace twsn
#endif
