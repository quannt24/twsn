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
#include "channelmgr.h"
#include "basemobility.h"
#include "coord.h"

namespace twsn {

Define_Module(BaseWirelessPhy);

void BaseWirelessPhy::initialize()
{
    // Call initialize() of parent
    BasePhy::initialize();

    // TODO Register with ChannelMgr at stage 0
    channelMgr = check_and_cast<ChannelMgr*>(getModuleByPath("channelMgr"));
    registerChannel();
}

void BaseWirelessPhy::initialize(int stage)
{
    EV << "BaseWirelessPhy::info: initialization stage " << stage << endl;

    switch (stage) {
        case 0:
            initialize();
            break;
        case 1:
            break;
        default:
            // Get list of adjacent nodes (which are in txRange of this node)
            if (channelMgr != NULL) adjPhyList = channelMgr->getAdjPhyList(getId());
            break;
    }
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

void BaseWirelessPhy::registerChannel()
{
    if (channelMgr == NULL) return;

    BaseMobility *mob = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));

    channelMgr->registerChannel(getId(), mob->getCoord(), (distance_t) par("txRange").doubleValue());
    EV << "BaseWirelessPhy::info: Register channel\n";
}

BaseWirelessPhy::BaseWirelessPhy()
{
    channelMgr = NULL;
}

}
