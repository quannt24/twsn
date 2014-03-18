/*
 * coord.cc
 *
 *  Created on: Mar 11, 2014
 *      Author: quannt
 */

#include "coord.h"
#include "math.h"

namespace twsn {

distance_t distance(Coord c1, Coord c2)
{
    return sqrt((c1.x - c2.x) * (c1.x - c2.x)
            + (c1.y - c2.y) * (c1.y - c2.y)
            + (c1.z - c2.z) * (c1.z - c2.z));
}

}
