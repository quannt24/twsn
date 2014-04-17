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

#ifndef __TWSN_BASESIMPLE_H_
#define __TWSN_BASESIMPLE_H_

#include <omnetpp.h>
#include "twsndef.h"

namespace twsn {

enum ErrLevel {VERBOSE, DEBUG, INFO, WARNING, ERROR};

/**
 * Base class for simple module of a host.
 */
class BaseSimple : public cSimpleModule
{
    protected:
        void printError(ErrLevel errType, const std::string &msg);
};

}

#endif
