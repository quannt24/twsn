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

#ifndef __TWSN_ACOUSTICSENSOR_H_
#define __TWSN_ACOUSTICSENSOR_H_

#include <omnetpp.h>
#include "basesimple.h"
#include "gaussiannoise.h"
#include "measurement.h"
#include "sensemsg_m.h"

namespace twsn {

/**
 * Acoustic sensor for tracking
 */
class AcousticSensor : public BaseSimple
{
    private:
        GaussianNoise *noise;
        int nSensedTarget; // Number of sensed targets
        int nMeasurement; // Number of measurements currently created
        /* Measurement list, storing measurements of a sensing action. This list will be cleared
         * before executing new sensing. */
        std::list<Measurement> meaList;

        cMessage *responseTimer; // Timer for responding measurement

        /** Start sensing action. Request nearby target to send corresponding signals/data. */
        void startSense();
        /** Process sensed signal */
        void processSignal(SenseSignal *ss);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        AcousticSensor();
        ~AcousticSensor();
};

}  // namespace twsn

#endif
