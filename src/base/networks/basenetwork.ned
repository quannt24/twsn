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

package twsn.base.networks;

network BaseNetwork
{
    parameters:
        double width @unit(m) = default(50m);
        double height @unit(m) = default(50m);
        // Devide network into a grid
        int rows = default(1);
        int cols = default(1);
        
        @display("bgs=3,m;bgb=$width,$height;"); // 1m ~ 3px
}
