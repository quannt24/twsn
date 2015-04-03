/*
 * targettrace.h
 *
 *  Created on: Apr 6, 2014
 *      Author: Quan T. Nguyen
 */

#ifndef TARGETTRACE_H_
#define TARGETTRACE_H_

#include "targetpos.h"
#include <list>

namespace twsn {

class TargetTrace
{
    private:
        int id;
        std::list<TargetPos> path;
        double pVx, pVy; // Predicted velocity. Unit m/s
        double theta; // Theta for Critically Damped g-h filter (<1)
        double minDeltaT; // Minimum delta t allowed to update velocity

    public:
        /**
         * TargetTrace use Critically Damped g-h filter to estimate tracking path.
         * 'theta' parameter (0 <= theta <= 1) specify fading speed of value of history data. When
         * theta = 0, the history data is not used; when theta = 1, we use only history data to
         * predict next position. Default value is 0.5.
         * 'minDeltaT' is the threshold of difference between timestamps of two consecutive
         * TargetPos for updating velocity. If the difference is too small, it will multiply the
         * error in estimation of velocity. This parameter should be specified based on particular
         * system.
         */
        TargetTrace(double theta = 0.5, double minDeltaT = 0.2);

        int getId() const { return id; }
        void setId(int id) { this->id = id; }

        /**
         * Add a TargetPos to path.
         * This function does not check if the TargetPos is acceptable or not. It will use passed
         * TargetPos as new measurement to track. The actual TargetPos added to pass is calculated
         * (filtered) and returned by the function.
         */
        TargetPos addTargetPos(TargetPos tp);
        /**
         * Get number of TargetPos in path
         */
        int getPathLen() { return path.size(); }
        /**
         * Check likelihood value of a TargetPos with this trace.
         * A distanceThreshold is needed to reject TargetPos which having to too large distance to
         * predicted position.
         * A timeThreshold is used to reject TargetPos which having too large time difference
         * with current trace. If timeThreshold = 0, this function will accept any non-negative
         * time difference.
         * Returned value is >= 0 if the TargetPos is acceptable at some degree; the bigger returned
         * value, the unlikely TargetPos belongs to this trace. If TargetPos is unacceptable, -1 is
         * returned.
         */
        double checkLikelihood(TargetPos tp, double distanceThreshold, double timeThreshold = 0);
        /**
         * Get tracked path
         */
        std::list<TargetPos> getPath() { return path; }
};

} /* namespace twsn */
#endif /* TARGETTRACE_H_ */
