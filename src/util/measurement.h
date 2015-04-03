/*
 * measurement.h
 *
 *  Created on: Oct 4, 2013
 *      Author: quannt
 */

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include "coord.h"

namespace twsn {

/*
 * A measurement of distance to a target
 */
class Measurement
{
    private:
        int tarId; // Target ID
        double measuredDistance; // Measured distance (not true distance)
        double trueDistance; // True distance from node to target (for reference)
        Coord tarCoord; // True position of target (for reference)
        Coord nodeCoord; // Position of sensor node
        double nodeEnergy; // Remaining energy of sensor

        /* Initial function for constructors */
        void init(int tarId, double md, Coord tarCoord, Coord nodeCoord, double nodeEnergy);

    public:
        Measurement();
        Measurement(int tarId, double md, Coord tarCoord);
        Measurement(int tarId, double md, Coord tarCoord, Coord nodeCoord, double nodeEnergy);

        int getTarId() { return tarId; }
        void setTarId(int tarId) { this->tarId = tarId; }

        double getMeasuredDistance() { return measuredDistance; }
        void setMeasuredDistance(double measuredDistance) { this->measuredDistance = measuredDistance; }

        double getTrueDistance() const { return trueDistance; }
        void setTrueDistance(double trueDistance) { this->trueDistance = trueDistance; }

        Coord getTarCoord() { return tarCoord; }
        void setTarCoord(Coord coord) { this->tarCoord = coord; }

        Coord getNodeCoord() { return nodeCoord; }
        void setNodeCoord(Coord coord) {this->nodeCoord = coord; }

        double getNodeEnergy() { return nodeEnergy; }
        void setNodeEnergy(double nodeEnergy) { this->nodeEnergy = nodeEnergy; }
};

}  // namespace twsn

#endif /* MEASUREMENT_H_ */
