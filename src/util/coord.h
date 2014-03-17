/*
 * coord.h
 *
 *  Created on: Mar 11, 2014
 *      Author: Quan T. Nguyen
 */

#ifndef COORD_H_
#define COORD_H_

#include "twsndef.h"

BEGIN_NAMESPACE_TWSN

/**
 * Coordination in pixel
 */
class Coord
{
    private:
        double x, y, z;

    public:
        /** Constructors */
        Coord() { x = 0; y = 0; z = 0; };
        Coord(double x, double y = 0, double z = 0) {
            this->x = x;
            this->y = y;
            this->z = z;
        };

        double getX() const { return x; }
        void setX(double x) { this->x = x; }

        double getY() const { return y; }
        void setY(double y) { this->y = y; }

        double getZ() const { return z; }
        void setZ(double z) { this->z = z; }

        friend distance_t distance(Coord c1, Coord c2);
};

distance_t distance(Coord c1, Coord c2);

END_NAMESPACE_TWSN

#endif /* COORD_H_ */
