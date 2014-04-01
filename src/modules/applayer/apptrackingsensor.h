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

#ifndef __TWSN_APPTRACKINGSENSOR_H_
#define __TWSN_APPTRACKINGSENSOR_H_

#include <omnetpp.h>
#include "baseapp.h"
#include "sensemsg_m.h"

namespace twsn {

/**
 * Application for tracking sensor node
 */
class AppTrackingSensor : public BaseApp
{
    private:
        cMessage *senseTimer; // Self message for start sensing

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void handleSelfMsg(cMessage *msg);
        virtual void handleSenseMsg(SenseMsg *msg);

    public:
        AppTrackingSensor();
        ~AppTrackingSensor();
};

}  // namespace twsn

#endif
