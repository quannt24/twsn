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
// Interface for wireless physical module
//
moduleinterface IWirelessPhy extends IPhy
{
    parameters:
        // For simplicity, all physical modules are assumed having the same signal sensitivity,
        // so that we can simulate a fixed transmission range for each physical module.
        // This parameter should be configured corresponding with output power (dBm) and
        // sensitivity of simulated devices with corresponding current consumption.
        double txRange @unit(m);

        // Radio mode at start of simulation
        // enum RadioMode {POWER_DOWN = 0, IDLE = 1, RX = 2, TX = 3};
        int initRadioMode;

        // Radio mode switching delay
        double delayIdleToRx @unit(s);
        double delayIdleToTx @unit(s);

        double delayRxToIdle @unit(s);
        double delayRxToTx @unit(s);

        double delayTxToIdle @unit(s);
        double delayTxToRx @unit(s);

        // Power consumption
        double pIdle @unit(mW);
        double pRx @unit(mW);
        double pTx @unit(mW);
        // Period for planning timer to calculate and draw energy from power source (simulate
        // continuous draw)
        double drawPeriod;

        @display("i=block/wrxtx");
    gates:
        input radioIn @directIn; // for sendDirect from other physical layers
}
