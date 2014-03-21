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
    
    // Physical specific commands
    PHYCMD_PD = 2; // Power down
    PHYCMD_IDLE = 3; // Switch to IDLE mode
    PHYCMD_RX = 4; // Switch to RX mode
    PHYCMD_TX = 5; // Switch to TX mode
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