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

package twsn.modules.linklayer;

//
// Link layer with XT-MAC protocol
//
simple LinkBMAC extends LinkUnslottedCSMACA
{
    parameters:
        @class("twsn::LinkBMAC");
		
		// Check interval (radio is in idle mode), greater sleep interval save more rx energy
		// but causes more delay and txt energy
		double checkInterval @unit(s) = default(0.1 s);
		// Preamble length, should equal 'byteRoundup(bitRate * (checkInterval + turnAroundTime + ccaDur)' 
        int preambleLen @unit(byte) = default(3135 byte);
        // Timeout to stay active without any activities
        double activeTime @unit(s) = default(2s);
}
