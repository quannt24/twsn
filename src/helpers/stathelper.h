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

#ifndef __TWSN_STATHELPER_H_
#define __TWSN_STATHELPER_H_

#include <omnetpp.h>
#include "basesimple.h"

namespace twsn {

/**
 * Statistics helper
 */
class StatHelper : public BaseSimple
{
    private:
        /* Statistic signals */
        simsignal_t sigMeaError;

    protected:
        virtual void initialize();

    public:
        /** Record measurement error (measurement of distance) */
        void recMeaError(double err);
        /** Record estimation (filtered) error (measurement of distance) */
        void recEstError(double err);
};

}  // namespace twsn

#endif
