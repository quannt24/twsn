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

#include "worldhelper.h"
#include "basemobility.h"
#include "basenet.h"

namespace twsn {

Define_Module(WorldHelper);

void WorldHelper::arrangeNodes()
{
    cModule *wsn = getModuleByPath("^");
    double wsnWidth = wsn->par("width");
    double wsnHeight = wsn->par("height");
    int wsnRows = wsn->par("rows");
    int wsnCols = wsn->par("cols");
    int numNodes = wsn->par("numNodes");
    cModule *node;
    BaseMobility *mob;

    int i;
    for (i = 0; i < numNodes; i++) {
        node = wsn->getSubmodule("node", i);
        mob = check_and_cast<BaseMobility*>(node->getSubmodule("mobility"));

        //std::cerr << (i % wsnCols) * (wsnWidth / wsnCols) + intuniform(0, wsnWidth / wsnCols) << ' ';
        //std::cerr << (i / wsnCols % wsnRows) * (wsnHeight / wsnRows) + intuniform(0, wsnHeight / wsnRows) << endl;
        mob->setCoordX((i % wsnCols) * (wsnWidth / wsnCols) + intuniform(0, wsnWidth / wsnCols * 3 / 4));
        mob->setCoordY((i / wsnCols % wsnRows) * (wsnHeight / wsnRows) + intuniform(0, wsnHeight / wsnRows * 3 / 4));
        mob->setRow(i / wsnCols % wsnRows);
        mob->setCol(i % wsnCols);
        mob->updateDisplay(); // Update display of sensor node
    }
}

void WorldHelper::informNodes()
{
    cModule *wsn = getModuleByPath("^");
    BaseMobility *bsMob = check_and_cast<BaseMobility*>(getModuleByPath("^.bs.mobility"));
    BaseNet *nodeNetLayer;
    int numNodes = wsn->par("numNodes");
    int i;

    for (i = 0; i < numNodes; i++) {
        nodeNetLayer = check_and_cast<BaseNet*>(wsn->getSubmodule("node", i)->getSubmodule("net"));
        nodeNetLayer->setBsPos(bsMob->getCoord());
    }
}

void WorldHelper::initialize()
{
    arrangeNodes();
    informNodes();
}

}
