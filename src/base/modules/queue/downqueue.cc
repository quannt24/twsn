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

#include "downqueue.h"

namespace twsn {

Define_Module(DownQueue);

void DownQueue::dequeueMsg()
{
    if (!queue.isEmpty()) {
        cPacket *pkt = queue.pop();
        sendDown(pkt);
    }
}

void DownQueue::handleUpperMsg(cMessage* msg)
{
    // Add to queue and notify lower layer
    queue.insert(check_and_cast<cPacket*>(msg));
    notifyLower();
}

void DownQueue::handleUpperCtl(cMessage* msg)
{
    // Just forward to lower layer
    sendCtlDown(check_and_cast<Command*>(msg));
}

void DownQueue::handleLowerMsg(cMessage* msg)
{
    // Just forward to upper layer
    sendUp(check_and_cast<cPacket*>(msg));
}

void DownQueue::handleLowerCtl(cMessage* msg)
{
    Command *cmd = check_and_cast<Command*>(msg);

    switch (cmd->getCmdId()) {
        case CMD_DATA_FETCH:
            // Dequeue message and send down
            dequeueMsg();
            delete cmd;
            break;
        default:
            // Just forward to upper layer
            sendCtlUp(cmd);
            break;
    }
}

}  // namespace twsn
