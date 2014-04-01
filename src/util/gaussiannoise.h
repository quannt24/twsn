/*
 * gaussiannoise.h
 *
 *  Created on: Nov 7, 2013
 *      Author: quannt
 */

#ifndef GAUSSIANNOISE_H_
#define GAUSSIANNOISE_H_

namespace twsn {

class GaussianNoise
{
    private:
        double muy;
        double sigma;
    public:
        GaussianNoise(double muy, double sigma);
        double nextNoise();
};

}  // namespace twsn

#endif /* GAUSSIANNOISE_H_ */
