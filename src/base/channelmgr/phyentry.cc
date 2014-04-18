/*
 * phyentry.cc
 *
 *  Created on: Mar 17, 2014
 *      Author: Quan T. Nguyen
 */

#include "phyentry.h"

namespace twsn {

int PhyEntry::getChannelState() const
{
    return channelState;
}

void PhyEntry::setChannelState(int state)
{
    channelState = state;
    if (channelState > 0) ccaResult = false;
}

/** Increase number of in-air transmission signal at the position of this node */
void PhyEntry::incChannelState()
{
    channelState++;
    if (channelState > 0) ccaResult = false;
}

/** Decrease number of in-air transmission signal at the position of this node */
void PhyEntry::decChannelState()
{
    channelState--;
    if (channelState < 0) channelState = 0;
}

void PhyEntry::clearAdjList()
{
    adjList->clear();
}

void PhyEntry::addAdjNode(PhyEntry* pe)
{
    adjList->push_back(pe);
}

/** Start performing CCA */
void PhyEntry::startCCA()
{
    if (channelState == 0) {
        ccaResult = true;
    } else {
        ccaResult = false;
    }
}

/** Finish CCA, return CCA result: true if channel is clear */
bool PhyEntry::finishCCA()
{
    return ccaResult;
}

}
