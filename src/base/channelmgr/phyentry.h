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

BEGIN_NAMESPACE_TWSN

class PhyEntry
{
    private:
        moduleid_t moduleId; // ID of physical module
        Coord coord; // Position of the device
        distance_t txRange; // Transmission range
        int index; // Index in entry list, this index will not be changed if the module re-registers

    public:
        PhyEntry() {
            moduleId = 0;
            coord = Coord();
            txRange = 0;
        }
        PhyEntry(moduleid_t moduleId, Coord &coord, distance_t txRange, int index) {
            this->moduleId = moduleId;
            this->coord = coord;
            this->txRange = txRange;
            this->index = index;
        }

        inline moduleid_t getModuleId() const { return moduleId; }
        inline void setModuleId(moduleid_t moduleId) { this->moduleId = moduleId; }

        inline const Coord& getCoord() const { return coord; }
        inline void setCoord(const Coord& coord) { this->coord = coord; }

        inline distance_t getTxRange() const { return txRange; }
        inline void setTxRange(distance_t txRange) { this->txRange = txRange; }

        inline int getIndex() const { return index; }
        inline void setIndex(int index) { this->index = index; }
};

END_NAMESPACE_TWSN

#endif /* PHYENTRY_H_ */
