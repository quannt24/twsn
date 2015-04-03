//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __TWSN_BASEMOBILITY_H_
#define __TWSN_BASEMOBILITY_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "basesimple.h"
#include "coord.h"

namespace twsn {

/**
 * Base class for all mobility modules, which provides logic about position and mobility of an Entity.
 */
class BaseMobility : public BaseSimple
{
    private:
        /** Position (coordinate in meter), used to place module on inspector */
        Coord coord;
        /** Position in row/column (just for reference only, not for actual drawing) */
        int row, col;

    protected:
        virtual void initialize();

    public:
        Coord getCoord() { return coord; };
        void setCoord(Coord coord) { this->coord = coord; };
        void setCoord(double x, double y) { coord.setX(x); coord.setY(y); };
        double getCoordX() { return coord.getX(); };
        void setCoordX(double x) { coord.setX(x); };
        double getCoordY() { return coord.getY(); };
        void setCoordY(double y) { coord.setY(y); };

        int getRow() { return row; };
        void setRow(int r) { row = r; };
        int getCol() { return col; };
        void setCol(int c) { col = c; };

        /** Update display of parent module in simulation */
        void updateDisplay();
};

}

#endif
