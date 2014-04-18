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
        /** Number of signals being sent over position of the node (>= 0) */
        int channelState;
        std::list<PhyEntry*> *adjList; // List of entries which is in-range of this node
        bool performingCCA; // Flag on if performing CCA
        bool ccaResult; // CCA result, 'true' if channel is clear

    public:
        PhyEntry() {
            moduleId = 0;
            coord = Coord();
            txRange = 0;
            channelState = 0;
            adjList = new std::list<PhyEntry*>; // Init an empty list
            performingCCA = false;
            ccaResult = true;
        }
        PhyEntry(moduleid_t moduleId, Coord coord, distance_t txRange) {
            this->moduleId = moduleId;
            this->coord = coord;
            this->txRange = txRange;
            this->channelState = 0;
            this->adjList = new std::list<PhyEntry*>; // Init an empty list
            performingCCA = false;
            ccaResult = true;
        }
        virtual ~PhyEntry() { delete adjList; }

        moduleid_t getModuleId() const { return moduleId; }
        void setModuleId(moduleid_t moduleId) { this->moduleId = moduleId; }

        const Coord getCoord() const { return coord; }
        void setCoord(const Coord& coord) { this->coord = coord; }

        distance_t getTxRange() const { return txRange; }
        void setTxRange(distance_t txRange) { this->txRange = txRange; }

        int getChannelState() const;
        void setChannelState(int state);
        /** Increase number of in-air transmission signal at the position of this node */
        void incChannelState();
        /** Decrease number of in-air transmission signal at the position of this node */
        void decChannelState();

        /** Get list of adjacent nodes (PhyEntry) */
        std::list<PhyEntry*>* getAdjList() { return adjList; }
        /** Clear list of adjacent nodes */
        void clearAdjList();
        /** Add a node to adjacent list */
        void addAdjNode(PhyEntry *pe);

        /** Start performing CCA */
        void startCCA();
        /** Finish CCA, return CCA result: true if channel is clear */
        bool finishCCA();
};

}

#endif /* PHYENTRY_H_ */
