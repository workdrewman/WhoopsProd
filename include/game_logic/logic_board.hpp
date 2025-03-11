/// logic_board.hpp
#ifndef GAME_BOARD_LOGIC_BOARD_HPP
#define GAME_BOARD_LOGIC_BOARD_HPP

#include <vector>

using namespace std;

namespace logic {
    class LogicPlayer;
    class LogicTerminal;

    const vector<int> kSlideStartLocations = {0, 6, 11, 17, 22, 28, 33, 39};
    const vector<int> kSlideEndLocations = {3, 9, 14, 20, 25, 31, 36, 42};
    const vector<int> kSafetyLocations = {47, 48, 49, 56, 57, 58, 65, 66, 67, 74, 75, 76};
    const vector<int> kHomeLocations = {44, 45, 46, 53, 54, 55, 62, 63, 64, 71, 72, 73};
    const vector<int> kStartLocations = {50, 51, 52, 59, 60, 61, 68, 69, 70, 77, 78, 79};
    const int kBoardSize = 80;

    class LogicBoard {
        private:
            int MAX_PLAYERS = 4;
            int PIECES_PER_PLAYER = 3;
            int MAX_PIECES = MAX_PLAYERS * PIECES_PER_PLAYER;

        public:
            LogicBoard() : currentLocations(kBoardSize, 0), lastLocations(kBoardSize, 0) {};

            vector<int> currentLocations;
            vector<int> lastLocations;
            
            vector<int> findOtherPawnLocations(LogicPlayer* Player, int currentLocation);

            void movePiece(int startIndex, int endIndex);
            bool isCollision(int location, int playerColor);
            bool checkWinCondition(LogicPlayer* Player);

            int findNextOpenHome(int color);
            int findNextOpenStart(int color);
            int checkSlide(LogicPlayer* Player, int location);

            bool allPiecesPlaced();
            bool allPiecesOnStart(LogicPlayer* Player, LogicTerminal* Terminal);
    };
}

#endif // GAME_BOARD_LOGIC_BOARD_HPP