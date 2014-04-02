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
#include "macpkt_m.h"

namespace twsn {

/**
 * Link layer with unslotted CSMA/CA
 */
class LinkUnslottedCSMACA : public BaseLink
{
    protected:
        MacPkt *outPkt; // Packet being sent

        int aMaxBE;
        int macMaxNB;
        int macMinBE;
        int nb; // Number of current backoff
        int be; // Backoff exponent

        /* Timers */
        cMessage *backoffTimer; // End of backoff
        cMessage *listenTimer; // Start listen
        cMessage *fetchTimer; // Timer for fetching packet from queue

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

        /** Start sending procedures to send outPkt */
        void startSending();
        /** Backoff with random backoff expenent */
        void backoff();
        /** Request physical layer to perform CCA for a specific duration */
        void performCCA();
        /** Update variables and begin next round */
        void nextRound();
        /** Reset outPkt and switch to RX mode */
        void reset();

    public:
        LinkUnslottedCSMACA();
        ~LinkUnslottedCSMACA();
};

}  // namespace twsn

#endif
