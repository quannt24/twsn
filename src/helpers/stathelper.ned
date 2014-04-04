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

package twsn.helpers;

import twsn.base.modules.BaseSimple;

//
// Statistics helper
//
simple StatHelper extends BaseSimple
{
    parameters:
        @class("twsn::StatHelper");

		// Measurement error of distance
        @signal[sigMeaError](type="double");
        @statistic[meaError](title="Measured error"; source="sigMeaError"; record=vector,histogram);
        
        // Estimation error of distance
        @signal[sigPosError](type="double");
        @statistic[posError](title="Positioning error"; source="sigPosError"; record=vector,histogram);
        
        @display("i=block/table2");
}
