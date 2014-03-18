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
        std::list<PhyEntry> peList;
        /**
         * Channel Access Table (CAT), storing channel access states of registered physical modules.
         * Each row is manipulated by corresponding transmitter.
         */
        int **channelAccessTbl;

        /** Initialize channel access table */
        void initCAT();

    protected:
        /** Override to use 3 initialization stages */
        virtual int numInitStages () const { return 3; };
        virtual void initialize(); // Stage 0
        virtual void initialize(int stage);

    public:
        ChannelMgr();
        virtual ~ChannelMgr();

        /**
         * Register physical module with this ChannelMgr module.
         * All physical module using this ChannelMgr module must call this function to register in
         * the initialization stage 0.
         * If a registered physical module calls this function or it's not initialization stage 0 at
         * the time this function is called, there will be no effect.
         */
        void registerChannel(moduleid_t moduleId, Coord &coord, distance_t txRange);
};

}

#endif
