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

package twsn.base.modules.phylayer;

//
// Base module for wireless physical layer
//
simple BaseWirelessPhy extends BasePhy like IWirelessPhy
{
    parameters:
        @class("BaseWirelessPhy");
        
        // For simplicity, all physical modules are assumed having the same signal sensitivity,
        // so that we can simulate a fixed transmission range for each physical module.
        // This parameter should be configured corresponding with output power (dBm) and
        // sensitivity of simulated devices with corresponding current consumption.
        double txRange @unit(m);
        
        // Current consumption
        double currTx @unit(mA);
        double currRx @unit(mA);
        
        int initRadioMode; // Radio mode at start of simulation (0 = Idle; 1 = Rx; 2 = Tx)
        
        // Radio mode switching delay
        double delayIdleToRx @unit(s);
        double delayIdleToTx @unit(s);
        
        double delayRxToIdle @unit(s);
        double delayRxToTx @unit(s);
        
        double delayTxToIdle @unit(s);
        double delayTxToRx @unit(s);
        
    gates:
        input radioIn @directIn; // for sendDirect from other physical layers   
}
