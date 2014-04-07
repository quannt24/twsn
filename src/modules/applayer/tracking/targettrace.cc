/*
 * targettrace.cc
 *
 *  Created on: Apr 6, 2014
 *      Author: Quan T. Nguyen
 */

#include "targettrace.h"

namespace twsn {

TargetTrace::TargetTrace()
{
    id = 0;
    predictedVx = 0;
    predictedVy = 0;
}

void TargetTrace::addTargetPos(TargetPos tp)
{
    if (path.size() == 0) {
        // Just add to path
        predictedVx = 0;
        predictedVy = 0;
    } else {
        TargetPos &recentPos = path.back();
        double dVx = (tp.getCoord().getX() - recentPos.getCoord().getX())
                / (tp.getTimestamp() - recentPos.getTimestamp());
        double dVy = (tp.getCoord().getY() - recentPos.getCoord().getY())
                        / (tp.getTimestamp() - recentPos.getTimestamp());
        // History fades with rate = 0.5
        predictedVx = (predictedVx + dVx) / 2;
        predictedVy = (predictedVy + dVy) / 2;
    }
    path.push_back(tp);
}

double TargetTrace::checkLikelihood(TargetPos tp, double distanceThreshold, double timeThreshold)
{
    if (path.size() == 0) return 0; // Perfect match

    TargetPos &recentPos = path.back();
    if (tp.getTimestamp() < recentPos.getTimestamp()) return -1;

    double dt = tp.getTimestamp() - recentPos.getTimestamp();
    if (timeThreshold > 0 && dt > timeThreshold) return -1;

    Coord predictedPos = Coord(recentPos.getCoord().getX() + predictedVx * dt,
                               recentPos.getCoord().getY() + predictedVy * dt);

    double likely = distance(predictedPos, tp.getCoord());
    if (likely <= distanceThreshold) {
        return likely;
    } else {
        return -1;
    }
}

} /* namespace twsn */
