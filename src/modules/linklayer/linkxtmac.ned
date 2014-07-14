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
simple LinkXTMAC extends LinkUnslottedCSMACA
{
    parameters:
        @class("twsn::LinkXTMAC");
        
        // If disabled, node will not go to inactive state. Strobes still be sent as normal.
        bool enableDutyCycling = default(true);
        
		// Sender
		// ===================================
		// Strobe time is estimated sending time of a MAC packet without payload and
		// having only ONE worst backoff.
		// strobeTime = aTurnaroundTime + 2^macMinBE * aUnitBP + ccaDur + phyFrameSize / bitRate
		double strobeTime @unit(s) = default(aTurnaroundTime + 8 * aUnitBP + ccaDur + 0.000864 s); // about 0.003744 s
		// Reserved interval at end of each strobe period, not configurable
		// reservedInterval = strobeTime (about 0.003744 s in default)
		double reservedInterval @unit(s) = strobeTime;
		// Period for sending strobes. Must larger reservedInterval + sendingTime.
		double strobePeriod @unit(s) = default(strobeTime + reservedInterval); // about 0.007488 s
		// Deadline (in each period) for sending a strobe. If the deadline is miss, the strobe will
		// be ignored, not configurable.
		double strobeDeadline @unit(s) = strobePeriod - reservedInterval;
		
		// Receiver
		// ===================================
		// Listen interval (seconds), must long enough to receive one of two consecutive strobes
		// Minimum value can be set is equal 2 * strobePeriod - reservedInterval
		double listenInterval @unit(s) = default(2 * strobePeriod - reservedInterval);
		// Sleep interval (radio is in idle mode), greater sleep interval save more rx energy
		// but causes more delay
		double sleepInterval @unit(s) = default(0.2 s);
		
        // Active time length
        double activeTime @unit(s) = default(2 s);
        
        // Timeout for cancel main packet sending (should around traffic load)
        double sendTimeout @unit(s) = default (0.5 s);
}
