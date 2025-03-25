/// logic_terminal.hpp
#ifndef GAME_BOARD_LOGIC_TERMINAL_HPP
#define GAME_BOARD_LOGIC_TERMINAL_HPP

#include <vector>
#include <iostream>

#include "rfid/rfid_scanner.hpp"

namespace logic {
    class LogicBoard;
    class RfidScanner;
    class LogicPlayer;  
    class LogicCalculations;
    class LogicTerminal {
        public:
            LogicTerminal();
            int t_GetChip();
            int t_setStartAreas(LogicBoard* Board, LogicPlayer* Player);
            void t_displayChipInstructions(rfid::RfidScanner* Scanner);
            void t_selectPiece(LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, int chip);
    };
    int readIntFromSerial();
}

#endif // GAME_BOARD_LOGIC_TERMINAL_HPP