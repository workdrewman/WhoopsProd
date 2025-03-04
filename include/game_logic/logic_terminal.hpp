/// logic_terminal.hpp
#ifndef GAME_BOARD_LOGIC_TERMINAL_HPP
#define GAME_BOARD_LOGIC_TERMINAL_HPP

#include <vector>
#include <iostream>

namespace logic {
    class LogicBoard;
    class LogicChip;
    class LogicPlayer;  
    class LogicCalculations;
    class LogicTerminal {
        public:
            LogicTerminal();
            int t_GetChip();
            int t_setStartAreas(LogicBoard* Board, LogicPlayer* Player);
            void t_displayChipInstructions(LogicChip* Chip);
            void t_whereAreMyPieces(LogicBoard* Board, LogicPlayer* Player);
            void t_selectPiece(LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc);
    };
}

#endif // GAME_BOARD_LOGIC_TERMINAL_HPP