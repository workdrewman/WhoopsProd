/// logic_player.hpp
#ifndef GAME_BOARD_LOGIC_PLAYER_HPP
#define GAME_BOARD_LOGIC_PLAYER_HPP

#include <vector>
#include <iostream>

#include "rfid/rfid_scanner.hpp"

using namespace std;

namespace logic {
    class LogicBoard;
    class LogicTerminal;
    class LogicPlayer {
        private:
            int playerCount;

        public:
            LogicPlayer();
            int getPlayerColor(int playerIndex) const;
            void setPlayerColor(int movingFrom, int color, int player);
            int getPlayerCount();
            void setPlayerCount(rfid::RfidScanner* Scanner, LogicTerminal* Terminal);
            void handleSelfCollision(LogicBoard* Board, int possibleMove, vector<int> &possibleMoves, int index, int chip, int movingFrom);

            int currentPlayer;
            vector<int> playerColors = {0, 0, 0, 0}; 
    };
} 

#endif // GAME_BOARD_LOGIC_PLAYER_HPP