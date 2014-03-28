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

import twsn.base.modules.linklayer.BaseLink;

//
// Link layer with unslotted CSMA/CA
//
simple LinkUnslottedCSMACA extends BaseLink
{
    parameters:
        @class("twsn::LinkUnslottedCSMACA");
        
        double symbolPeriod @unit(s) 		= 0.000016 s;
        double ccaDur @unit(s) 				= default(8 * symbolPeriod);
        
        int aMaxBE 							= default(5);
        int aMaxSIFSFrameSize @unit(byte) 	= default(18 byte);
        double aMinSIFSPeriod @unit(s) 		= default(12 * symbolPeriod);
        double aMinLIFSPeriod @unit(s) 		= default(40 * symbolPeriod);
        double aTurnaroundTime @unit(s) 	= default(12 * symbolPeriod);
        double aUnitBP @unit(s) 			= default(20 * symbolPeriod);
		int macMaxNB 						= default(4); // 0 ~ 5, default 4
		int macMinBE 						= default(3); // 0 ~ 3, default 3
}
