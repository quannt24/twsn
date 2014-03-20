/*
 * phyentry.h
 *
 *  Created on: Mar 17, 2014
 *      Author: Quan T. Nguyen
 */

#ifndef PHYENTRY_H_
#define PHYENTRY_H_

#include "twsndef.h"
#include "coord.h"
#include <list>

namespace twsn {

class PhyEntry
{
    private:
        moduleid_t moduleId; // ID of physical module
        Coord coord; // Position of the device
        distance_t txRange; // Transmission range
        int channelState; // Number of in-air signals at position of the node >= 0
        std::list<PhyEntry*> *adjList; // List of entries which is in-range of this node

    public:
        PhyEntry() {
            moduleId = 0;
            coord = Coord();
            txRange = 0;
            channelState = 0;
            adjList = new std::list<PhyEntry*>; // Init an empty list
        }
        PhyEntry(moduleid_t moduleId, Coord coord, distance_t txRange) {
            this->moduleId = moduleId;
            this->coord = coord;
            this->txRange = txRange;
            this->channelState = 0;
            this->adjList = new std::list<PhyEntry*>; // Init an empty list
        }
        virtual ~PhyEntry() { delete adjList; }

        moduleid_t getModuleId() const { return moduleId; }
        void setModuleId(moduleid_t moduleId) { this->moduleId = moduleId; }

        const Coord getCoord() const { return coord; }
        void setCoord(const Coord& coord) { this->coord = coord; }

        distance_t getTxRange() const { return txRange; }
        void setTxRange(distance_t txRange) { this->txRange = txRange; }

        int getChannelState() const { return channelState; }
        void setChannelState(int state) { this->channelState = state; }
        /** Increase number of in-air transmission signal at the position of this node */
        void incChannelState() { channelState++; }
        /** Decrease number of in-air transmission signal at the position of this node */
        void decChannelState()
        {
            channelState--;
            if (channelState < 0) channelState = 0;
        }

        /** Get list of adjacent nodes (PhyEntry) */
        std::list<PhyEntry*>* getAdjList() { return adjList; }
        /** Clear list of adjacent nodes */
        void clearAdjList();
        /** Add a node to adjacent list */
        void addAdjNode(PhyEntry *pe);
};

}

#endif /* PHYENTRY_H_ */
