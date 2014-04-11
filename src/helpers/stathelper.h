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
        cMessage *pollTRETimer;

        /* Statistic signals */
        simsignal_t sigMeaError;
        simsignal_t sigPosError; // Positioning error produced by tracking algorithm
        simsignal_t sigTRE; // Total residual energy of sensor nodes
        simsignal_t sigRecvMacPkt; // Packet received at link layer
        simsignal_t sigLostMacPkt; // Packet lost at link layer and physical layer
        simsignal_t sigRecvNetPkt; // Packet received at network layer
        simsignal_t sigLostNetPkt; // Packet lost at network layer

        /** Poll total residual energy */
        void pollTotalResEnergy();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        StatHelper();
        ~StatHelper();

        /** Record measurement error (measurement of distance) */
        void recMeaError(double err);
        /** Record positioning error produced by tracking algorithm */
        void recPosError(double err);
        /** Emit a signal with value 1 to count number of received MAC packets */
        void countRecvMacPkt();
        /** Emit a signal with value 1 to count number of lost MAC packets */
        void countLostMacPkt();
        /** Emit a signal with value 1 to count number of received network packets */
        void countRecvNetPkt();
        /** Emit a signal with value 1 to count number of lost network packets */
        void countLostNetPkt();
};

}  // namespace twsn

#endif
