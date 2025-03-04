/// logic_special.hpp
#ifndef GAME_BOARD_LOGIC_SPECIAL_HPP
#define GAME_BOARD_LOGIC_SPECIAL_HPP

#include <vector>
#include <iostream>

using namespace std;

namespace logic {
    class RfidScanner;
    class LogicBoard;
    class LogicPlayer;  
    class LogicCalculations;
    class LogicSpecial {
        public:
            LogicSpecial();
            void handleWhoops(RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves);
            void handleSeven(RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves, int movingFrom);
            void moveSecondPawn(LogicBoard* Board, LogicPlayer* Player, int distance, int start);
            void handleEleven(RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, vector<int> possibleMoves, int movingFrom);
    };
}

#endif // GAME_BOARD_LOGIC_SPECIAL_HPP