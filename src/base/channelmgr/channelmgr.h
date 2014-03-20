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

#ifndef __TWSN_CHANNELMGR_H_
#define __TWSN_CHANNELMGR_H_

#include <omnetpp.h>
#include "twsndef.h"
#include "basesimple.h"
#include "phyentry.h"
#include "coord.h"
#include "airframe_m.h"

namespace twsn {

/** States used in channel access table */
enum CATState{CAT_FREE, CAT_BUSY, CAT_OUT_OF_RANGE = -1};

/**
 * Central module for simulating wireless channel
 */
class ChannelMgr : public BaseSimple
{
    private:
        int currInitStage;
        // List of PhyEntry object, contains info of registered physical modules
        std::list<PhyEntry*> peList;
        /** Air frames being sent */
        std::list<AirFrame*> afList;

    protected:
        /** Override to use multiple initialization stages */
        virtual int numInitStages () const { return 3; };
        virtual void initialize(int stage);

    public:
        ChannelMgr();
        virtual ~ChannelMgr();

        /**
         * Called by physical module at initialization stage 1.
         * Register physical module with this ChannelMgr module.
         * All physical module using this ChannelMgr module must call this function to register in
         * the initialization stage 1 (after their positions are specified).
         * If it's not initialization stage 0 at the time this function is called, there will be no
         * effect.
         * NOTE: EACH MODULE SHOULD ONLY CALL THIS FUNCTION ONLY ONE TIME; because of performance,
         * we do not check if an physical module has already registered and duplicated entries may
         * cause unexpected behaviors.
         * Return: a pointer to the registered PhyEntry object corresponding to the physical module.
         */
        PhyEntry* registerChannel(moduleid_t moduleId, Coord coord, distance_t txRange);

        /**
         * Called by a node when starting transmission. Change (increase) channel state of sending
         * node and its adjacent nodes (if they are in tx range of the sender, whether or not they
         * are going to receive an air frame). Current in-air frames (in afList) will be checked
         * for error because of interference.
         */
        void startTx(PhyEntry *sender);
        /**
         * Called by a node when it finishes its transmission. Change (decrease) channel state of
         * sending node and its adjacent nodes (if they are in tx range of the sender, whether or
         * not they are going to receive an air frame).
         */
        void stopTx(PhyEntry *sender);
        /**
         * Called by a node sending an air frame.
         * Hold an air frame which is being sent to the air. It will be checked for error caused
         * by interference due to current channel state of receiver.
         * Sender must make sure receiving module is in its txRange (in its adjList).
         * Error checking will be wrong if receiver is not in adjList of sender (this behavior is
         * for performance).
         */
        void holdAirFrame(PhyEntry *sender, AirFrame *frame);

        /**
         * Called by every node finishing receiving an air frame.
         * Release an air frame when it is received by a node.
         */
        void releaseAirFrame(AirFrame *frame);
};

}

#endif
