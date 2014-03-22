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
import twsn.base.modules.phylayer.IWirelessPhy;
import twsn.base.modules.linklayer.ILink;

// Base module for all wireless nodes
module BaseWirelessNode extends BaseEntity like INode
{
    parameters:
        string phyType = default("BaseWirelessPhy");
        string linkType = default("BaseLink");

        @display("is=s");
    submodules:
        phy: <phyType> like IWirelessPhy;
        link: <linkType> like ILink;
    connections:
        phy.upper <--> link.lower;
        phy.upperCtl <--> link.lowerCtl;
}
