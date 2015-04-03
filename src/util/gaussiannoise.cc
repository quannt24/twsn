/*
 * gaussiannoise.cc
 *
 *  Created on: Nov 7, 2013
 *      Author: quannt
 */

#include "gaussiannoise.h"
#include <omnetpp.h>
#include <math.h>

namespace twsn {

GaussianNoise::GaussianNoise(double muy, double sigma)
{
    this->muy = muy;
    this->sigma = sigma;
}

/*
 * Generate Next Gaussian using Box Muller transform
 * x ~ N(muy, sigma^2)
 * s,t ~ Uniform distribution: [0; 1]
 */
double GaussianNoise::nextNoise()
{
    double x = 0, s, t;
    do {
        s = uniform(0, 1);
        t = uniform(0, 1);
    } while (s == 0);

    x = muy + sigma * sqrt(-2 * log(s)) * cos(2 * M_PI * t);
    return x;
}

}  // namespace twsn
