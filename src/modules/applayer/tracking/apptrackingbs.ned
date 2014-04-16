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

package twsn.modules.applayer.tracking;

import twsn.base.modules.applayer.BaseApp;

//
// Application layer of tracking Base Station
//
simple AppTrackingBS extends BaseApp
{
    parameters:
        @class("twsn::AppTrackingBS");
        
        // Theta parameter for Critically Damped g-h filter
        double theta = default(0.5);
        // Min delta t for updating estimated velocity
        double minDeltaT @unit(s) = default(0.2 s);
        // Distance (compared to predicted position) threshold for adding a position to a trace
        double distanceThreshold @unit(m) = default(20 m);
        // Time (compared to timestamp of most recent tracked position) threshold for adding a position to a trace
        double timeThreshold @unit(s) = default(2 s);

		// Min junk trace old
		double minJunkTraceOld @unit(s) = default(3 s);
		// Max junk trace length
		int maxJunkTraceLen = default(5);
		
		@signal[sigEtoEDelay](type="double");
        @statistic[EtoEDelay](title="End to end delay"; source="sigEtoEDelay"; record=vector);
        @signal[sigTrackError](type="double");
        @statistic[trackError](title="Track error"; source="sigTrackError"; record=vector);
}
