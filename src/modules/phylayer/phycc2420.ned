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

package twsn.modules.phylayer;

import twsn.base.modules.phylayer.BaseWirelessPhy;

simple PhyCC2420 extends BaseWirelessPhy
{
    parameters:
        // Radio mode switching delay
        delayIdleToRx @unit(s) = default(0.000192 s); // 12 symbol periods
        delayIdleToTx @unit(s) = default(0.000192 s);

        delayRxToIdle @unit(s) = default(0 s);
        delayRxToTx @unit(s) = default(0.000192 s);

        delayTxToIdle @unit(s) = default(0 s);
        delayTxToRx @unit(s) = default(0.000192 s);

        // Power consumption
        pIdle @unit(mW) = default(1.278 mW);
        pRx @unit(mW) = default(56.4 mW);
        pTx @unit(mW) = default(52.2 mW);
}