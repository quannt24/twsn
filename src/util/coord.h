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
        int x, y, z;

    public:
        /** Constructors */
        Coord();
        Coord(int x, int y = 0, int z = 0);

        int getX() const { return x; }

        void setX(int x) { this->x = x; }

        int getY() const { return y; }

        void setY(int y) { this->y = y; }

        int getZ() const { return z; }

        void setZ(int z) { this->z = z; }
};

END_NAMESPACE_TWSN

#endif /* COORD_H_ */
