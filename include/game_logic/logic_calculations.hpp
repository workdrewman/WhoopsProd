/// logic_calculations.hpp
#ifndef GAME_BOARD_LOGIC_CALCULATIONS_HPP
#define GAME_BOARD_LOGIC_CALCULATIONS_HPP

#include <vector>

using namespace std;

namespace logic {
    class LogicBoard;
    class LogicPlayer;
    class LogicCalculations {
        public:
            LogicCalculations();
            int movingFrom;

            vector<int> findPossibleMoves(LogicBoard* Board, LogicPlayer* Player, int pieceIndex, int distance);
            void checkSecondPawnDistance(LogicBoard* Board, LogicPlayer* Player, int distances, int firstPawn, vector<int> &possibleMoves);
            int getDistance(LogicPlayer* Player, int start, int end);
    };
}

#endif // GAME_BOARD_LOGIC_CALCULATIONS_HPP