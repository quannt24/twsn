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

#ifndef __TWSN_DECOHELPER_H_
#define __TWSN_DECOHELPER_H_

#include <omnetpp.h>
#include "basesimple.h"

namespace twsn {

/**
 * Helper module for decoration
 */
class DecoHelper : public BaseSimple
{
    protected:
        virtual void initialize();

    public:
        /**
         * Create directional connection between two modules.
         * Output gate created by this function is prefixed with "dout" and input gate is prefixed
         * with "din".
         */
        void dConnect(cModule *from, cModule *to, const char *displayStr);
        /**
         * Remove all decorating output and connections of a module. Corresponding input gate on the
         * other will also be deleted.
         */
        void disconnect(cModule *mod);
};

}  // namespace twsn

#endif
