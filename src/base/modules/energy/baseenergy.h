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

#ifndef __TWSN_BASEENERGY_H_
#define __TWSN_BASEENERGY_H_

#include <omnetpp.h>
#include "basesimple.h"

namespace twsn {

const double INFINITE_ENERGY = -1;

/**
 * Base module for energy
 */
class BaseEnergy : public BaseSimple
{
    protected:
        // Capacity in mWh, non-negative except that -1 represent infinitive (INFINITE_ENERGY)
        double capacity;

        /* Statistics signals */
        simsignal_t sigResEnergy; // Residual energy

        virtual void initialize();

    public:
        /** Get current capacity in mWh */
        double getCapacity();
        /** Set capacity (usually by helper). Use mWh */
        void setCapacity(double cap);
        /**
         * Draw from energy source. Amount is in mWh.
         * If drawn amount is negative, the capacity of energy source has no change.
         * Return: Energy amount which is really drawn, which is non-negative and less than or equal
         * current capacity. If 'amount' is negative, 0 is returned and capacity of the source has
         * no change. If 'amount' is greater than current capacity, the current capacity is returned
         * and capacity of the source is set to zero.
         */
        double draw(double amount);
};

}  // namespace twsn

#endif
