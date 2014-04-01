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

package twsn.modules.other;

import twsn.base.modules.BaseSimple;

//
// Acoustic sensor for tracking
//
simple AcousticSensor extends BaseSimple
{
    parameters:
        @class("twsn::AcousticSensor");
        
        // Delay for processing signal before return sense result
        double responseDelay = default(0.05);
        @display("i=block/wrx");

    gates:
        input senseIn @directIn; // Gate for receiving target's signal
        inout appGate;
}
