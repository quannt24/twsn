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

#ifndef __TWSN_BASENET_H_
#define __TWSN_BASENET_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "baselayer.h"
#include "coord.h"

namespace twsn {

/**
 * Base module for network layer
 */
class BaseNet : public BaseLayer
{
    protected:
        macaddr_t macAddr; // MAC address of this node

        // Stored information about base station, these info must be set by helper so that node
        // can work properly.
        Coord bsCoord; // Base station position
        double dBs; // Distance from this node to base station

        /** Override to use multiple initialization stages */
        virtual int numInitStages() const { return 2; }
        virtual void initialize(int stage);
        virtual void initialize();

    public:
        BaseNet();

        /**
         * Preset position of BS. In simulation, helper can call this function to preset position
         * of BS to nodes in network.
         */
        void setBsPos(Coord bsCoord);
};

}  // namespace twsn

#endif
