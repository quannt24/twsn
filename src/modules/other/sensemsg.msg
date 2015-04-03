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

cplusplus {{
    #include "measurement.h"
}}

namespace twsn;

cplusplus {{
    typedef std::list<Measurement> MeasurementList;
}}

class noncobject Measurement; // announcement for the message compiler
class noncobject MeasurementList;

enum SenseMsgType {
    SS_START = 0; // Command from application to trigger sensing
    SS_CANCEL = 1; // Command from application to cancel sensing
    SS_RESULT = 2; // Contain sensing result to send back to application
    SS_ACTION = 3; // Sensing action sent to target to request for signal
    SS_SIGNAL = 4; // Acoustic signal from target
};

//
// Message for sensing
//
message SenseMsg {
    int msgType;
}

//
// Sensing result to send back to application
// 
message SenseResult extends SenseMsg {
    msgType = SS_RESULT;
    MeasurementList meaList; // Measurement list of targets
}

message SenseAction extends SenseMsg {
    msgType = SS_ACTION;
    int sensorId; // Module ID of sensor
    int mobilityId; // Module ID of mobility module of the node
}

//
// Message carrying signal of a target
//
message SenseSignal extends SenseMsg {
    msgType = SS_SIGNAL;
    
    int tarId; // Target ID (used for reference and when target classification is not available)
    //string signal; // Acoustic signal
    double distance; // True distance from target to sensing sensor (for reference)
    double x; // True x coordination (for reference)
    double y; // True y coordination (for reference)
}