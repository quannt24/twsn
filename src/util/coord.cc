/*
 * coord.cc
 *
 *  Created on: Mar 11, 2014
 *      Author: quannt
 */

#include "coord.h"

USING_NAMESPACE_TWSN

Coord::Coord()
{
    x = 0; y = 0; z = 0;
}

Coord::Coord(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
