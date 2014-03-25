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

#include "basesimple.h"

namespace twsn {

Define_Module(BaseSimple);

void BaseSimple::printError(ErrLevel level, const std::string &msg)
{
    std::string levelName;

    switch (level) {
        case VERBOSE:
            levelName = "Verbose";
            break;
        case DEBUG:
            levelName = "Debug";
            break;
        case INFO:
            levelName = "Info";
            break;
        case WARNING:
            levelName = "Warning";
            break;
        case ERROR:
            levelName = "Error";
            break;
        default:
            levelName = "Unknown";
            break;
    }

    std::cerr << getName() << '(' << getId() << ')' << ':' << levelName << ": " << msg << endl;
}

}
