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
		
		bool enableDutyCycling = default(false);
		// Check interval (radio is in idle mode), greater sleep interval save more rx energy
		// but causes more delay and txt energy
		double checkInterval @unit(s) = default(0.05 s);
		// Preamble length, should equal 'byteRoundup(bitRate * (checkInterval + turnAroundTime + ccaDur)'
		// 0.01s ~ 323 byte
		// 0.02s ~ 635 byte
		// 0.05s ~ 1573 byte
		// 0.1s ~ 3135 byte
		// 0.2s ~ 6260 byte
        int preambleLen @unit(byte) = default(1573 byte);
        // Timeout to stay active without any activities, must be greater worst sending time of a
        // packet (including LIFS and around 0.032672)
        // In general, longer listenTimeout may be more reliable, but consumpts more energy
        double listenTimeout @unit(s) = default(0.08 s);
}
