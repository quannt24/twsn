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

namespace twsn {

/**
 * Network layer using ARPEES protocol
 */
class NetARPEES : public BaseNet
{
    protected:
        // MAC addresses for routing
        // Value 0 means connection info is not initialized.
        int bsAddr; // Base station
        int rnAddr; // Relay node

        // Stored information about relay node
        double enerRn; // Energy
        double dRnBs; // Distance from relay node to base station in meter
        double dRn; // Distance from relay node to this node

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

    public:
        NetARPEES();
        ~NetARPEES();
};

}  // namespace twsn
#endif
