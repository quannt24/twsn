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

#ifndef __TWSN_BASELAYER_H_
#define __TWSN_BASELAYER_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "basesimple.h"
#include "command_m.h"

namespace twsn {

/**
 * Base module for a layer
 */
class BaseLayer : public BaseSimple
{
    protected:
        /** Delegate jobs to other message handling functions. */
        virtual void handleMessage(cMessage *msg);

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

        /** Send data packet up */
        virtual void sendUp(cPacket *pkt);
        /** Send control message up */
        virtual void sendCtlUp(Command *cmd);
        /** Send data packet down */
        virtual void sendDown(cPacket *pkt);
        /** Send control message down */
        virtual void sendCtlDown(Command *cmd);
};

} // namespace twsn

#endif
