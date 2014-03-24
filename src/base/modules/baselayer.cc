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

#include "baselayer.h"

namespace twsn {

Define_Module(BaseLayer);

void BaseLayer::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGate() == gate("upper$i")) {
        handleUpperMsg(msg);
    } else if (msg->getArrivalGate() == gate("upperCtl$i")) {
        handleUpperCtl(msg);
    } else if (msg->getArrivalGate() == gate("lower$i")) {
        handleLowerMsg(msg);
    } else if (msg->getArrivalGate() == gate("lowerCtl$i")) {
        handleLowerCtl(msg);
    }
}

void BaseLayer::handleSelfMsg(cMessage* msg)
{
    delete msg;
}

void BaseLayer::handleUpperMsg(cMessage* msg)
{
    delete msg;
}

void BaseLayer::handleUpperCtl(cMessage* msg)
{
    delete msg;
}

void BaseLayer::handleLowerMsg(cMessage* msg)
{
    delete msg;
}

void BaseLayer::handleLowerCtl(cMessage* msg)
{
    delete msg;
}

void BaseLayer::sendUp(cPacket* pkt)
{
    if (gate("upper$o")->isPathOK()) {
        send(pkt, "upper$o");
    } else {
        printError(ERROR, "Gate is not connected. Deleting message.");
        delete pkt;
    }
}

void BaseLayer::sendCtlUp(Command* cmd)
{
    if (gate("upperCtl$o")->isPathOK()) {
        send(cmd, "upperCtl$o");
    } else {
        printError(ERROR, "Gate is not connected. Deleting message.");
        delete cmd;
    }
}

void BaseLayer::sendDown(cPacket* pkt)
{
    send(pkt, "lower$o");
}

void BaseLayer::sendCtlDown(Command* cmd)
{
    send(cmd, "lowerCtl$o");
}

void BaseLayer::fetchPacketFromUpper()
{
    Command *cmd = new Command();
    cmd->setCmdId(CMD_DATA_FETCH);
    sendCtlUp(cmd);
}

void BaseLayer::notifyLower()
{
    Command *cmd = new Command();
    cmd->setCmdId(CMD_DATA_NOTI);
    sendCtlDown(cmd);
}

} // namespace twsn
