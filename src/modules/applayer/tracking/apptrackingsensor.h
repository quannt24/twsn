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
#include "targetpos.h"

namespace twsn {

/**
 * Application for tracking sensor node
 */
class AppTrackingSensor : public BaseApp
{
    private:
        // Tracking filter
        double fR, fVr; // Filtered R and Vr (changing speed of R)
        int fRound; // Round number of filter, reset to 0 when have new target

        bool syncSense; // Synchronization state of sensing round
        bool hasMeasurement; // Indicate that the node have measurement of current cycle or not
        double senseTimestamp; // Time stamp of recent sensing action. It's the time application receives measurement from sensor
        Measurement measurement; // Measurement of recent sensing of this node
        std::list<Measurement> meaCollection; // List of collected measurements in cluster

        cMessage *senseTimer; // Self message for start sensing
        cMessage *reportTimer; // Timer for reporting measurement
        cMessage *collTimer; // Timer for collecting measurements
        cMessage *chBeaconTimer; // Timer for CH broadcasts beacon

        /** Request adjacent nodes to synchronize sensing cycle */
        void requestSyncSense();
        /** Send sense result to CH (broadcast, CH will collect these result) */
        void broadcastMeasurement();
        /** Promote this node to CH (if appropriate). Then estimate targets' positions (if is CH). */
        void promoteCH();
        /**
         * Estimate target position using Lateration algorithm.
         * Return a pointer to dynamically allocated TargetPos object or NULL if estimation fails.
         * Returned TargetPos object should be freed manually.
         */
        TargetPos *estimatePosition(std::list<Measurement> &meaList);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        virtual void handleSelfMsg(cMessage *msg);
        virtual void handleLowerMsg(cMessage *msg);
        virtual void handleSenseMsg(SenseMsg *msg);

    public:
        AppTrackingSensor();
        ~AppTrackingSensor();
};

}  // namespace twsn

#endif
