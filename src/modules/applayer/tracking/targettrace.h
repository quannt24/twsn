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
        double predictedVx, predictedVy; // Predicted velocity. Unit m/s

    public:
        TargetTrace();

        int getId() const { return id; }
        void setId(int id) { this->id = id; }

        /**
         * Add a TargetPos to path.
         * This function does not check if the TargetPos is acceptable or not.
         */
        void addTargetPos(TargetPos tp);
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
