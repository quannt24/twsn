/*
 * targettrace.cc
 *
 *  Created on: Apr 6, 2014
 *      Author: Quan T. Nguyen
 */

#include "targettrace.h"
#include <iostream>

namespace twsn {

TargetTrace::TargetTrace(double theta, double minDeltaT)
{
    id = 0;
    pVx = 0;
    pVy = 0;
    this->minDeltaT = minDeltaT;
    if (0 <= theta && theta <= 1) {
        this->theta = theta;
    } else {
        this->theta = 0.5;
    }
}

void TargetTrace::addTargetPos(TargetPos tp)
{
    if (path.size() == 0) {
        // Just add to path
        path.push_back(tp);
        // Initialize predicted values
        pVx = 0;
        pVy = 0;
    } else {
        // Predict
        TargetPos &recentPos = path.back();
        Coord recentCoord = recentPos.getCoord();
        double dt = tp.getTimestamp() - recentPos.getTimestamp(); // Delta t
        double pX = recentCoord.getX() + dt * pVx; // Newly predicted x
        double pY = recentCoord.getY() + dt * pVy; // Newly predicted y

        // Update
        Coord meaCoord = tp.getCoord(); // Measured coordinate from sensor node (CH)
        // Update velocity
        /* Because of nature of system, delta t can vary. Too small delta t can occur and multiply
         * estimation error. Therefore, we only update velocity when delta t is not too small. */
        if (dt >= minDeltaT) {
            pVx = pVx + (1 - theta) * (1 - theta) * (meaCoord.getX() - pX) / dt;
            pVy = pVy + (1 - theta) * (1 - theta) * (meaCoord.getY() - pY) / dt;
            std::cerr << "Updated velocity " << pVx << ' ' << pVy << ' ' << dt << '\n';
        }
        // Update new position
        Coord newCoord;
        newCoord.setX(pX + (1 - theta * theta) * (meaCoord.getX() - pX));
        newCoord.setY(pY + (1 - theta * theta) * (meaCoord.getY() - pY));

        TargetPos filteredPos;
        filteredPos.setCoord(newCoord);
        filteredPos.setTimestamp(tp.getTimestamp());
        path.push_back(filteredPos);
    }
}

double TargetTrace::checkLikelihood(TargetPos tp, double distanceThreshold, double timeThreshold)
{
    if (path.size() == 0) return 0; // Perfect match

    TargetPos &recentPos = path.back();

    double dt = tp.getTimestamp() - recentPos.getTimestamp();
    if (dt < 0 || (timeThreshold > 0 && dt > timeThreshold)) return -1;

    Coord predictedPos = Coord(recentPos.getCoord().getX() + pVx * dt,
                               recentPos.getCoord().getY() + pVy * dt);

    double likely = distance(predictedPos, tp.getCoord());
    if (likely <= distanceThreshold) {
        return likely;
    } else {
        return -1;
    }
}

} /* namespace twsn */
