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

BEGIN_NAMESPACE_TWSN

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
        inline Coord getCoord() { return coord; };
        inline void setCoord(Coord coord) { this->coord = coord; };
        inline void setCoord(double x, double y) { this->coord = Coord(x, y); };
        inline double getCoordX() { return coord.getX(); };
        inline void setCoordX(double x) { coord.setX(x); };
        inline double getCoordY() { return coord.getY(); };
        inline void setCoordY(double y) { coord.setY(y); };

        inline int getRow() { return row; };
        inline void setRow(int r) { row = r; };
        inline int getCol() { return col; };
        inline void setCol(int c) { col = c; };

        /** Update display of parent module in simulation */
        void updateDisplay();
};

END_NAMESPACE_TWSN

#endif
