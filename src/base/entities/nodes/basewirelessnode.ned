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

package twsn.base.entities.nodes;

import twsn.base.entities.BaseEntity;
import twsn.base.modules.energy.IEnergy;
import twsn.base.modules.phylayer.IWirelessPhy;
import twsn.base.modules.ILayer;
import twsn.base.modules.linklayer.ILink;
import twsn.base.modules.netlayer.INet;

//
// Base module for all wireless nodes
//
module BaseWirelessNode extends BaseEntity like INode
{
    parameters:
        string energyType = default("BaseEnergy");

        string phyType = default("BaseWirelessPhy");
        string linkType = default("BaseLink");
        string linkQueueType = default("DownQueue"); // Queue buffer uppon link layer
        string netType = default("BaseNet");
        string netQueueType = default("DownQueue");

        @display("is=s");
    submodules:
        energy: <energyType> like IEnergy {
            @display("p=50,150");
        }
        phy: <phyType> like IWirelessPhy {
            @display("p=150,50");
        }
        link: <linkType> like ILink {
            @display("p=150,150");
        }
        linkQueue: <linkQueueType> like ILayer {
            @display("p=150,250");
        }
        net: <netType> like INet {
            @display("p=150,350");
        }
        netQueue: <netQueueType> like ILayer {
            @display("p=150,450");
        }
    connections:
        phy.upper <--> link.lower;
        phy.upperCtl <--> link.lowerCtl;
        link.upper <--> linkQueue.lower;
        link.upperCtl <--> linkQueue.lowerCtl;
        linkQueue.upper <--> net.lower;
        linkQueue.upperCtl <--> net.lowerCtl;
        net.upper <--> netQueue.lower;
        net.upperCtl <--> netQueue.lowerCtl;
}
