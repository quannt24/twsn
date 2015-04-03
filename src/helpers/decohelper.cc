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

#include "decohelper.h"

namespace twsn {

Define_Module(DecoHelper);

void DecoHelper::initialize()
{
}

void DecoHelper::dConnect(cModule* from, cModule* to, const char *displayStr)
{
    cGate *out, *in;
    std::ostringstream ossOut, ossIn;

    ossOut << "doutTo" << to->getId() << '\0';
    ossIn << "dinFrom" << from->getId() << '\0';

    out = from->addGate(ossOut.str().c_str(), cGate::OUTPUT);
    in = to->addGate(ossIn.str().c_str(), cGate::INPUT);

    out->connectTo(in);

    // Set display string
    if (par("enable").boolValue()) {
        out->setDisplayString(displayStr);
    } else {
        out->setDisplayString("ls=#ffffff,0");
    }
}

void DecoHelper::disconnect(cModule* mod)
{
    std::vector<const char*> gateNames = mod->getGateNames();
    cGate *out, *in;

    for (std::vector<const char*>::iterator it = gateNames.begin(); it != gateNames.end(); it++) {
        if ((*it)[0] == 'd' && (*it)[1] == 'o') {
            out = mod->gate(*it);
            in = out->getNextGate();

            out->disconnect();
            mod->deleteGate(*it);
            in->getOwnerModule()->deleteGate(in->getName());
        }
    }
}

}  // namespace twsn
