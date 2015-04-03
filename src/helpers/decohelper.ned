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
// Helper module for decoration
//
simple DecoHelper extends BaseSimple
{
    parameters:
        @class("twsn::DecoHelper");
        
        // If enable = false, decoration will be hided
        bool enable = default(true);
        
        @display("i=block/network2");
}
