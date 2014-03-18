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

namespace twsn {

Define_Module(ChannelMgr);

void ChannelMgr::initialize()
{
    // Stage 0
}

void ChannelMgr::initialize(int stage)
{
    currInitStage = stage;
    EV << "ChannelMgr::info: initialization stage " << currInitStage << endl;

    switch (stage) {
        case 0:
            initialize();
            break;
        case 1:
            // Initialize channel access table
            initCAT();
            break;
        default:
            // Mark that initialization finished
            currInitStage = -1;
            break;
    }
}

void ChannelMgr::initCAT()
{
    if (peList.size() <= 0) {
        channelAccessTbl = NULL;
        return;
    }

    int listSize = peList.size();
    channelAccessTbl = new int*[listSize];
    int i, j;
    std::list<PhyEntry>::iterator peIt1, peIt2;

    for (i = 0, peIt1 = peList.begin(); i < listSize; i++, peIt1++) {
        channelAccessTbl[i] = new int[listSize]; // Allocate memory

        for (j = 0, peIt2 = peList.begin(); j < listSize; j++, peIt2++) {
            if (distance((*peIt1).getCoord(), (*peIt2).getCoord()) <= (*peIt1).getTxRange()) {
                channelAccessTbl[i][j] = CAT_FREE;
            } else {
                channelAccessTbl[i][j] = CAT_OUT_OF_RANGE;
            }
            // TODO print for test
        }
    }
}

ChannelMgr::ChannelMgr()
{
    currInitStage = -1; // Not in initialization
}

ChannelMgr::~ChannelMgr()
{
    unsigned int i;

    // Free CAT
    for (i = 0; i < peList.size(); i++) {
        delete channelAccessTbl[i];
    }
    delete channelAccessTbl;

    peList.clear();
}

void ChannelMgr::registerChannel(moduleid_t moduleId, Coord& coord, distance_t txRange)
{
    // Only allow register in initialization stage 0
    if (currInitStage != 0) {
        EV << "ChannelMgr::warning: Registration is not allowed when not in initialization stage 0\n";
        return;
    }

    std::list<PhyEntry>::iterator peIt;

    // Check if the module has already registered
    for (peIt = peList.begin(); peIt != peList.end(); peIt++) {
        if ((*peIt).getModuleId() == moduleId) {
            EV << "ChannelMgr::warning: Module is already registered\n";
            return;
        }
    }

    // Add new physical entry to list
    peList.push_back(PhyEntry(moduleId, coord, txRange, peList.size()));
}

} /* Namespace twsn */
