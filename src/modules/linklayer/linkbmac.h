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

#ifndef __TWSN_LINKBMAC_H_
#define __TWSN_LINKBMAC_H_

#include <omnetpp.h>
#include "linkunslottedcsmaca.h"

namespace twsn {

/**
 * Link layer with XT-MAC protocol
 */
class LinkBMAC : public LinkUnslottedCSMACA
{
    protected:
        bool active;
        bool forcedActive;

        cMessage *checkChannelTimer;
        cMessage *waitTurnaroundTimer;

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
         * Change to active state. If it is a forcedly activation (forced = true), layer will keep
         * active state for an interval specified by 'duration' before come back to normal duty
         * cycling. Passing a non-positive value to 'duration' when forced = true will keep active
         * state forever.
         */
        void activate(bool forced = false, double duration = 0);
        /**
         * Deactive, switch transceiver to IDLE mode.
         */
        void deactive();

        /**
         * Pop packet from queue and prepare sending if not currently in sending procedures.
         */
        void prepareQueuedPkt();
        /** Create a preamle MAC packet */
        Mac802154Pkt *createPreamble();

        /** Request physical layer to switch to RX mode if currently in IDLE */
        void switchToRx();
        /** Request physical layer to switch to IDLE mode */
        void switchToIdle();

    public:
        LinkBMAC();
        ~LinkBMAC();
};

}  // namespace twsn
#endif
