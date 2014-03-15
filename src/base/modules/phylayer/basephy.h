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

#ifndef __TWSN_BASEPHY_H_
#define __TWSN_BASEPHY_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "basesimple.h"

BEGIN_NAMESPACE_TWSN

/**
 * Base class for physical layer
 */
class BasePhy : public BaseSimple
{
    protected:
        /** Delegate jobs to other message handling functions. */
        virtual void handleMessage(cMessage *msg);

        /** Handle self message */
        virtual void handleSelfMsg(cMessage *msg);
        /** Handle message/packet from upperIn */
        virtual void handleUpperMsg(cMessage *msg);
        /** Handle control message from upperCtlIn */
        virtual void handleUpperCtl(cMessage *msg);
};

END_NAMESPACE_TWSN

#endif
