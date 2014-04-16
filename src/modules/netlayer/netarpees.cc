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

#include "netarpees.h"

namespace twsn {

Define_Module(NetARPEES);

void NetARPEES::handleSelfMsg(cMessage* msg)
{
}

void NetARPEES::handleUpperMsg(cMessage* msg)
{
}

void NetARPEES::handleUpperCtl(cMessage* msg)
{
}

void NetARPEES::handleLowerMsg(cMessage* msg)
{
}

void NetARPEES::handleLowerCtl(cMessage* msg)
{
}

NetARPEES::NetARPEES()
{
    bsAddr = 0;
    rnAddr = 0;

    waitRelayInfoTimer = new cMessage("waitRelayInfoTimer");
}

NetARPEES::~NetARPEES()
{
    cancelAndDelete(waitRelayInfoTimer);
}

}  // namespace twsn
