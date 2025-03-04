// =======================================================
// Description: This header file contains the declaration
//              of the Logic Controller class. This class is 
//              used to control the pawn and tile movements
// =======================================================

#ifndef GAME_BOARD_LOGIC_CONTROLLER_H
#define GAME_BOARD_LOGIC_CONTROLLER_H

// #include "../game_board/whoops_color.hpp" // for WhoopsColor
// #include "../game_board/pawn/pawn.hpp" // for Pawn
// #include "../game_board/tile/base_tile.hpp" // for Tile
// #include "../game_board/tile/tile_container.hpp" // for TileContainer

#include "logic_player.hpp"
#include "logic_board.hpp"
#include "logic_chip.hpp"
#include "logic_special.hpp"
#include "logic_calculations.hpp"
#include "logic_terminal.hpp"

#include <vector> // for std::vector
#include <memory> // for std::shared_ptr
#include <iostream> // for std::cout and std::endl
#include <algorithm> // for std::find

using namespace std;

namespace logic
{

    class LogicController {
        private:
            LogicPlayer Player;
            LogicBoard Board;
            LogicChip Chip;
            LogicSpecial Special;
            LogicCalculations Calc;
            LogicTerminal Terminal;

        public:
            LogicController();
            void startGame();
            void takeTurn();
            void nextPlayer();
    };

}

#endif // GAME_BOARD_CONTROLLER_H