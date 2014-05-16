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

#ifndef __TWSN_APPTRACKINGBS_H_
#define __TWSN_APPTRACKINGBS_H_

#include <omnetpp.h>
#include "baseapp.h"
#include "targettrace.h"

namespace twsn {

/**
 * Application layer of tracking Base Station
 */
class AppTrackingBS : public BaseApp
{
    private:
        std::list<TargetPos> rawTpList;
        std::list<TargetTrace> traceList;
        int numTrace; // Number of trace tracked (including noise)

        simsignal_t sigEtoEDelay;
        simsignal_t sigTrackError;

        /** Process newly added target position */
        void processTarPos(TargetPos &tarPos);
        /** Clean up junk traces */
        void cleanJunk();
        /** Output paths to files */
        void output();

    protected:
        virtual void initialize();
        virtual void handleLowerMsg(cMessage *msg);
        virtual void finish();
};

}  // namespace twsn
#endif
