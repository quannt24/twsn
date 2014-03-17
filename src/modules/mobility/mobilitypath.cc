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

#include "mobilitypath.h"
#include <fstream>

BEGIN_NAMESPACE_TWSN

Define_Module(MobilityPath);

void MobilityPath::initialize()
{
    // Call initialize() of parent
    BaseMobility::initialize();

    if (par("moving").boolValue()) {
        // Load path from file
        pathLen = loadPath() - 1;
        // Note: because of file reading, total number of read lines may be increase at end of file.
        // Therefore we discard the last.
        if (pathLen < 1) {
            EV<< "Target: path error";
            return;
        }

        posId = 0;
        setCoord(coordArr[posId]);

        // Schedule first move
        scheduleAt(par("startMovingTime").doubleValue(), moveMsg);

        // TODO Record position
    } else {
        // Static entity
        setCoord(par("posX").doubleValue(), par("posY").doubleValue());
    }

    // Update display
    updateDisplay();
}

void MobilityPath::handleMessage(cMessage *msg)
{
    // Self message: move
    // Move target to next position in path array
    posId++;
    if (posId < pathLen) {
        setCoord(coordArr[posId]);

        if (getCoordX() == 0 && getCoordY() == 0) {
            EV << "trouble here\n";
        }
        updateDisplay();
        scheduleAt(simTime() + par("movingTimeStep").doubleValue(), msg);

        // TODO Record position
    } else {
        if (par("stopSimWhenFinishMoving").boolValue()) {
            endSimulation();
        }
    }
}

/*
 * Load moving path from file.
 * Return number of coordinates read, -1 on error.
 */
int MobilityPath::loadPath()
{
    std::ifstream in(par("pathFile"));
    if (!in) {
        EV << "Cannot open path file " << par("pathFile").stringValue() << "\n";
        return -1;
    }

    int n = 0;
    double x, y;
    while (!in.eof() && n < MAX_PATH_LEN) {
        in >> x;
        in >> y;
        coordArr[n] = Coord(x, y);
        n++;
    }

    return n;
}

MobilityPath::MobilityPath()
{
    moveMsg = new cMessage("MoveMsg");
    coordArr = new Coord[MAX_PATH_LEN];
}

MobilityPath::~MobilityPath()
{
    cancelAndDelete(moveMsg);
    delete coordArr;
}

END_NAMESPACE_TWSN
