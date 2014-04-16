/*
 * targetpos.h
 *
 *  Created on: Apr 4, 2014
 *      Author: Quan T. Nguyen
 */

#ifndef TARGETPOS_H_
#define TARGETPOS_H_

#include "coord.h"

namespace twsn {

class TargetPos
{
    private:
        int tarId;
        Coord coord;
        Coord trueCoord; // True coordinate of target (for reference, not counted for packet size)
        double timestamp;

    public:
        static const int UNKNOWN_ID = -1;

        TargetPos() {
            tarId = UNKNOWN_ID;
            timestamp = 0;
        }

        int getTarId() const { return tarId; }
        void setTarId(int tarId) { this->tarId = tarId; }

        const Coord& getCoord() const { return coord; }
        void setCoord(const Coord& coord) { this->coord = coord; }

        const Coord& getTrueCoord() const { return trueCoord; }
        void setTrueCoord(const Coord& trueCoord) { this->trueCoord = trueCoord; }

        double getTimestamp() const { return timestamp; }
        void setTimestamp(double timestamp) { this->timestamp = timestamp; }
};

} /* namespace twsn */
#endif /* TARGETPOS_H_ */
