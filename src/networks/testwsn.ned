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

package twsn.networks;

import twsn.base.networks.BaseWSN;
import twsn.base.entities.IEntity;
import twsn.base.entities.nodes.INode;
import twsn.helpers.WorldHelper;

network TestWSN extends BaseWSN
{
    parameters:
        width = default(50 m);
        height = default(50 m);
        rows = default(2);
        cols = default(2);

        string nodeType = default("BaseWirelessNode"); // Sensor node type
        string bsType = default("TrackingBaseStation"); // Base station type
        
        int numNodes = default(4);

    submodules:
        node[numNodes]: <nodeType> like INode;
        bs: <bsType> like INode {
            parameters:
                mobility.posX = width / 2;
                mobility.posY = height;
        }

        // Helpers should be declared after all entities
        worldHelper: WorldHelper {
            @display("p=-20,10");
        }
}
