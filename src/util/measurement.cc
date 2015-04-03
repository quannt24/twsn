/*
 * measurement.cc
 *
 *  Created on: Oct 4, 2013
 *      Author: quannt
 */

#include "measurement.h"

namespace twsn {

/* Initial function for constructors */
void Measurement::init(int tarId, double md, Coord tarCoord, Coord nodeCoord, double nodeEnergy)
{
    this->tarId = tarId;
    this->measuredDistance = md;
    this->tarCoord = tarCoord;
    this->nodeCoord = nodeCoord;
    this->nodeEnergy = nodeEnergy;
}

Measurement::Measurement()
{
    this->tarId = -1;
    this->measuredDistance = 0;
    this->nodeEnergy = 0;
}

Measurement::Measurement(int tarId, double md, Coord tarCoord)
{
    init(tarId, md, tarCoord, Coord(), 0);
}

Measurement::Measurement(int tarId, double md, Coord tarCoord, Coord nodeCoord, double nodeEnergy)
{
    init(tarId, md, tarCoord, nodeCoord, nodeEnergy);
}

}  // namespace twsn
