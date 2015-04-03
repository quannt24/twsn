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
import twsn.helpers.DecoHelper;
import twsn.helpers.StatHelper;

network TrackingWSN extends BaseWSN
{
    parameters:
        width = default(400 m);
        height = default(400 m);
        rows = default(20);
        cols = default(20);
        
        string nodeType = default("TrackingNode");
        string bsType = default("TrackingBaseStation");
        string targetType = default("AcousticTarget");
        
        int numNodes = default(400);
        int numTargets = default(1);

    submodules:
        node[numNodes]: <nodeType> like INode; // Position of nodes will be arranged by helper
        bs: <bsType> like INode {
            parameters:
            	mobility.posX = width / 2;
            	mobility.posY = height;
        };
        target[numTargets]: <targetType> like IEntity;
        
        // Helpers should be declared after all entities
        worldHelper: WorldHelper {
            @display("p=-20,40");
        }
        decoHelper: DecoHelper {
            @display("p=-20,70");
        }
        statHelper: StatHelper {
            @display("p=-20,100");
        }
}
