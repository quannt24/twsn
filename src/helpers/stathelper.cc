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

#include "stathelper.h"

namespace twsn {

Define_Module(StatHelper);

void StatHelper::initialize()
{
    // Register signals
    sigMeaError = registerSignal("sigMeaError");
    sigEstError = registerSignal("sigEstError");
}

void StatHelper::recMeaError(double err)
{
    Enter_Method_Silent("recMeaError");
    emit(sigMeaError, err);
}

void StatHelper::recEstError(double err)
{
    Enter_Method_Silent("recEstError");
    emit(sigEstError, err);
}

}  // namespace twsn
