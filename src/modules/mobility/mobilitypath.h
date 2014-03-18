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

#ifndef __TWSN_MOBILITYPATH_H_
#define __TWSN_MOBILITYPATH_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "basemobility.h"

namespace twsn {

#define MAX_PATH_LEN 10000

/**
 * Mobility module with predefined discrete moving path
 */
class MobilityPath : public BaseMobility
{
    private:
        Coord *coordArr; // Path array
        int pathLen; // Number of point in path
        int posId; // Current index in path array

        cMessage *moveMsg; // Self message to move target to new position

        int loadPath(); // Load path from file to coordinate arrays

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        MobilityPath();
        ~MobilityPath();
};

}

#endif
