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

#include "channelmgr.h"
#include <iomanip>

namespace twsn {

Define_Module(ChannelMgr);

void ChannelMgr::initialize(int stage)
{
    currInitStage = stage;
    EV << "ChannelMgr::info: initialization stage " << currInitStage << endl;

    switch (stage) {
        case 0:
            BaseSimple::initialize(); // Stage 0
            // Positions of nodes are specified at this stage
            break;
        case 1:
            // Physical module registers at this stage
            break;
        default:
            // Mark that initialization finished
            currInitStage = -1;
            break;
    }
}

ChannelMgr::ChannelMgr()
{
    currInitStage = -1; // Not in initialization
}

ChannelMgr::~ChannelMgr()
{
    // Dispose graph of physical entries
    for (std::list<PhyEntry*>::iterator it = peList.begin(); it != peList.end(); it++) {
        delete (*it);
    }
    peList.clear();
}

PhyEntry* ChannelMgr::registerChannel(moduleid_t moduleId, Coord coord, distance_t txRange)
{
    Enter_Method_Silent("registerChannel");

    // Only allow register in an initialization stage after stage 0.
    // Calling this method is not allowed after initialization completes (currInitStage = -1).
    if (currInitStage < 1) {
        std::cerr << "ChannelMgr::error: registerChannel is not allowed at this time\n";
        return NULL;
    }

    std::list<PhyEntry*>::iterator it;

    distance_t d;
    // Create new physical entry
    PhyEntry *newPe = new PhyEntry(moduleId, coord, txRange);

    // Connect new entry with adjacent entries
    for (it = peList.begin(); it != peList.end(); it++) {
        if ((*it)->getModuleId() == moduleId) continue; // For preventing stupid bug self connected

        d = distance(coord, (*it)->getCoord());
        if (d < txRange) {
            newPe->addAdjNode(*it);
        }
        if (d < (*it)->getTxRange()) {
            (*it)->addAdjNode(newPe);
        }
    }

    peList.push_back(newPe);

    return newPe;
}

void ChannelMgr::startTx(PhyEntry *sender)
{
    Enter_Method_Silent("startTx");

    std::list<PhyEntry*>::iterator adjIt;
    std::list<AirFrame*>::iterator afIt;

    // Increase channel state of sender and adjacent nodes
    sender->incChannelState();
    for (adjIt = sender->getAdjList()->begin(); adjIt != sender->getAdjList()->end(); adjIt++) {
        if (*adjIt != sender) (*adjIt)->incChannelState();
    }

    // Consider all air frames being sent
    for (afIt = afList.begin(); afIt != afList.end(); afIt++) {

        /* Find entry of receiver of a frame and check its channel state (we only need to consider
         * receiving nodes which is in-range of newly transmitting node ('sender') including
         * 'sender' itself. */
        if ((*afIt)->getReceiver() == sender->getModuleId()) {
            (*afIt)->setBitError(true); // Of course, it's interfered here
        } else {
            for (adjIt = sender->getAdjList()->begin();
                    adjIt != sender->getAdjList()->end(); adjIt++) {
                if ((*adjIt)->getModuleId() == (*afIt)->getReceiver()) {
                    if ((*adjIt)->getChannelState() > 1) {
                        // If having interference (channel state > 1)
                        (*afIt)->setBitError(true);
                    }
                    break; // Next air frame in list
                }
            }
        }
    }
}

void ChannelMgr::stopTx(PhyEntry *sender)
{
    Enter_Method_Silent("stopTx");

    std::list<PhyEntry*>::iterator adjIt;

    // Decrease channel state of sender and adjacent nodes
    sender->decChannelState();
    for (adjIt = sender->getAdjList()->begin(); adjIt != sender->getAdjList()->end(); adjIt++) {
        if (*adjIt != sender) (*adjIt)->decChannelState();
    }
}

void ChannelMgr::holdAirFrame(PhyEntry *sender, AirFrame *frame)
{
    Enter_Method_Silent("holdAirFrame");

    std::list<PhyEntry*>::iterator adjIt;

    // Add frame to list of being sent frames
    afList.push_back(frame);

    // Check if this frame has error because of interference
    for (adjIt = sender->getAdjList()->begin(); adjIt != sender->getAdjList()->end(); adjIt++) {
        if ((*adjIt)->getModuleId() == frame->getReceiver()) {
            if ((*adjIt)->getChannelState() > 1) {
                // If having interference (channel state > 1) at receiver
                frame->setBitError(true);
            }
            break;
        }
    }
}

void ChannelMgr::releaseAirFrame(AirFrame* frame)
{
    Enter_Method_Silent("releaseAirFrame");

    std::list<AirFrame*>::iterator afIt;

    // Remove frame from afList
    for (afIt = afList.begin(); afIt != afList.end(); afIt++) {
        if (*afIt == frame) {
            afList.erase(afIt);
            break;
        }
    }
}

} /* Namespace twsn */
