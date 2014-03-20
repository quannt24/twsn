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
    // Stage 0
    // Call initialize() of parent
    BasePhy::initialize();
}

void BaseWirelessPhy::initialize(int stage)
{
    //EV << "BaseWirelessPhy::info: initialization stage " << stage << endl;
    switch (stage) {
        case 0:
            initialize();
            break;
        case 1:
            // Register with ChannelMgr at stage 1
            registerChannel();
            break;
    }
}

void BaseWirelessPhy::registerChannel()
{
    channelMgr = check_and_cast<ChannelMgr*>(getModuleByPath("channelMgr"));
    BaseMobility *mob = check_and_cast<BaseMobility*>(getModuleByPath("^.mobility"));
    if (channelMgr == NULL || mob == NULL) return;

    phyEntry = channelMgr->registerChannel(getId(), mob->getCoord(), (distance_t) par("txRange").doubleValue());
    EV << "BaseWirelessPhy::info: Register channel\n";
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
        handleAirFrame(check_and_cast<AirFrame*>(msg));
    }
}

void BaseWirelessPhy::handleSelfMsg(cMessage* msg)
{
    if (msg == finishTxTimer) {
        finishTx();
    }
}

void BaseWirelessPhy::handleAirFrame(AirFrame* frame)
{
    recvAirFrame(frame);
}

void BaseWirelessPhy::txMacPkt(MacPkt* pkt)
{
    if (phyEntry == NULL) {
        std::cerr << "BaseWirelessPhy::error: module has not registered with ChannelMgr\n";
        delete pkt;
        return;
    }
    /* Increase channel state */
    channelMgr->startTx(phyEntry);

    /* Create physical frame */
    AirFrame *frame = new AirFrame();
    frame->setByteLength(frame->getFrameSize()); // Physical frame size only
    frame->encapsulate(pkt); // Frame length will be increased by payload
    frame->setSender(getId());

    /* Calculate transmission time */
    simtime_t txTime = ((double) frame->getBitLength()) / par("bitRate").doubleValue();

    /* Send air frame containing MAC packet to each receiver */
    if (pkt->getDesAddr() == MAC_BROADCAST_ADDR) {
        // Send to all adjacent nodes
        std::list<PhyEntry*>::iterator adjIt;
        AirFrame *copy;

        for (adjIt = phyEntry->getAdjList()->begin(); adjIt != phyEntry->getAdjList()->end(); adjIt++) {
            copy = frame->dup();
            copy->setReceiver((*adjIt)->getModuleId());
            sendAirFrame(copy);
        }
        delete frame; // Original frame is redundant
    } else {
        frame->setReceiver(pkt->getDesAddr()); // MAC address is module id of physical module
        sendAirFrame(frame);
        // We do not simulate overhearing
    }

    /* Set timer for calling finishTx() */
    scheduleAt(simTime() + txTime, finishTxTimer);
}

void BaseWirelessPhy::finishTx()
{
    channelMgr->stopTx(phyEntry);
}

void BaseWirelessPhy::sendAirFrame(AirFrame* frame)
{
    simtime_t txTime = ((double) frame->getBitLength()) / par("bitRate").doubleValue();
    channelMgr->holdAirFrame(phyEntry, frame);
    sendDirect(frame, 0, txTime, simulation.getModule(frame->getReceiver()), "radioIn");
}

void BaseWirelessPhy::recvAirFrame(AirFrame* frame)
{
    channelMgr->releaseAirFrame(frame);
    delete frame; // TODO Decapsulate the frame and processing instead of just delete it
}

BaseWirelessPhy::BaseWirelessPhy()
{
    channelMgr = NULL;
    phyEntry = NULL;
    transmitting = false;

    finishTxTimer = new cMessage("FinishTxTimer");
}

BaseWirelessPhy::~BaseWirelessPhy()
{
    cancelAndDelete(finishTxTimer);
}

}
