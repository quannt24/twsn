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
        
        double ccaDur = 0.000128; // CCA duration (8 * symbolPeriod)
        double aUnitBP = 0.00032; // A unit backoff-period (20 * symbolPeriod)
        int aMaxBE = default(5);
		int macMaxNB = default(5);
		int macMinBE = default(3);
}
