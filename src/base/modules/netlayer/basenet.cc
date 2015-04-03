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

#include "basenet.h"
#include "baselink.h"
#include "basemobility.h"

namespace twsn {

Define_Module(BaseNet);

void BaseNet::initialize(int stage)
{
    switch (stage) {
        case 0:
            initialize();
            break;

        case 2:
            macAddr = check_and_cast<BaseLink*>(getModuleByPath("^.link"))->getMacAddr();
            char msg[100];
            sprintf(msg, "MAC address %d", (int) macAddr);
            printError(LV_VERBOSE, msg);
            break;
    }
}

void BaseNet::initialize()
{
    BaseLayer::initialize();
}

BaseNet::BaseNet()
{
    macAddr = 0;
}

void BaseNet::setBsPos(Coord bsCoord)
{
    BaseMobility *mob = (BaseMobility*) getModuleByPath("^.mobility");
    this->bsCoord = bsCoord;
    dBs = distance(bsCoord, mob->getCoord());
}

}  // namespace twsn
