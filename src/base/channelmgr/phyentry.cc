/*
 * phyentry.cc
 *
 *  Created on: Mar 17, 2014
 *      Author: Quan T. Nguyen
 */

#include "phyentry.h"

namespace twsn {

void twsn::PhyEntry::clearAdjList()
{
    adjList->clear();
}

void twsn::PhyEntry::addAdjNode(PhyEntry* pe)
{
    adjList->push_back(pe);
}

}
