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

namespace twsn;

// Level of module sending/receiving command
enum CmdLevel {
    UNKNOWN = 0;
    PHYS = 1;
    LINK = 2;
    NETW = 3;
    TRAN = 4;
    APPL = 5;
};

// Command ID of a control message, which specifies its function
enum CmdID {
    // Commnon commands
    CMD_DATA_NOTI = 0; // Notify lower layer for pending data packet
    CMD_DATA_FETCH = 1; // Notify upper layer to send data packet through data gate
    CMD_READY = 2; // Notify upper layer that lower layer is ready
    
    // Physical specific commands
    CMD_PHY_PD = 10; // Power down
    CMD_PHY_IDLE = 11; // Switch to IDLE mode
    CMD_PHY_RX = 12; // Switch to RX mode
    CMD_PHY_TX = 13; // Switch to TX mode
    CMD_PHY_CCA = 14; // Perform clear channel assessment (CCA)
    
    // Data link specific commands
    CMD_LIN_CCA_RESULT = 20; // CCA result
    CMD_LIN_ACTIVE = 21; // Notify link layer to stay active (with default active time)
    CMD_LIN_FORCE_ACTIVE = 22; // Force link layer stay active
};

// Base class for command messages
message Command {
    // Source and destination of command. These number is just for reference, a specific layer
    // can freely choose to forward command message or not.
    // Use CmdLevel
    int src = UNKNOWN;
    int des = UNKNOWN;
    // Command ID, use CmdID
    int cmdId;
}

// Request to perform clear channel assessment
message CmdCCA extends Command {
    src = LINK;
    des = PHYS;
    cmdId = CMD_PHY_CCA;
    double duration = 0.000128; // CCA duration, default is 8 symbol periods
}

// Clear channel assessment result
message CmdCCAR extends Command {
    src = PHYS;
    des = LINK;
    cmdId = CMD_LIN_CCA_RESULT;
    bool clearChannel;
}

// Force link layer stay active
message CmdForceActive extends Command {
    des = LINK;
    cmdId = CMD_LIN_FORCE_ACTIVE;
    double duration = 0; // Active duration in seconds, use 0 (or a value < 0) for infinite
}