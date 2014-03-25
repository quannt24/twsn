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

#include "baselink.h"

namespace twsn {

Define_Module(BaseLink);

void BaseLink::initialize(int stage)
{
    switch (stage) {
        case 0:
            initialized();
            break;

        case 1:
            macAddr = getModuleByPath("^.phy")->getId();
            char msg[100];
            sprintf(msg, "MAC address %d", (int) macAddr);
            printError(VERBOSE, msg);
            break;

        default:
            break;
    }
}

BaseLink::BaseLink()
{
    macAddr = 0;
}

}  // namespace twsn
