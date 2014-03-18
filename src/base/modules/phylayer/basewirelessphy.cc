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

#include "basewirelessphy.h"

namespace twsn {

Define_Module(BaseWirelessPhy);

void BaseWirelessPhy::initialize()
{
    // Call initialize() of parent
    BasePhy::initialize();

    // TODO Register with ChannelMgr
}

void BaseWirelessPhy::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGate() == gate("upper$i")) {
        handleUpperMsg(msg);
    } else if (msg->getArrivalGate() == gate("upperCtl$i")) {
        handleUpperCtl(msg);
    } else if (msg->getArrivalGate() == gate("radioIn")) {
        handleAirFrame(msg);
    }
}

void BaseWirelessPhy::handleAirFrame(cMessage* msg)
{
}

}
