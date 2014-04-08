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

package twsn.entities.nodes;

import twsn.base.entities.nodes.BaseWirelessNode;

//
// IEEE 802.15.4 complaint node
//
module Node802154 extends BaseWirelessNode
{
    parameters:
        energyType = default("BaseEnergy");

        phyType = default("PhyCC2420");
        linkType = default("LinkUnslottedCSMACA");
        linkQueueType = default("DownQueue"); // Queue buffer upon link layer
        netType = default("NetEMRP");
        netQueueType = default("DownQueue"); // Queue buffer upon network layer
}