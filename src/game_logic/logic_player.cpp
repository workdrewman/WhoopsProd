/// logic_player.cpp

#include "game_logic/logic_player.hpp"
#include "game_logic/logic_board.hpp"
#include "rfid/rfid_scanner.hpp"
#include "game_logic/logic_terminal.hpp"

#include <FastLED.h>

using namespace std;

namespace logic {
    LogicPlayer::LogicPlayer() {}

    int LogicPlayer::getPlayerColor(int playerIndex) const {
        return playerColors[playerIndex];
    }

    //POSSIBLE BUG: if player picks up a piece that already has a color
    void LogicPlayer::setPlayerColor(int movingFrom, int color, int player) {
        if (movingFrom >= 50 && movingFrom <= 52) {
            color = 1;    //Yellow
        } else if (movingFrom >= 59 && movingFrom <= 61) {
            color = 2;    //Green
        } else if (movingFrom >= 68 && movingFrom <= 70) {
            color = 3;    //Red
        } else if (movingFrom >= 77 && movingFrom <= 79) {
            color = 4;    //Blue
        }

        playerColors[player] = color;
    }

    int LogicPlayer::getPlayerCount() {
        return playerCount;
    }

    void LogicPlayer::setPlayerCount(rfid::RfidScanner* Scanner, LogicTerminal* Terminal) {
        //Scan chip to get player count
        Serial.println("Get number of players: ");
        //Audio
        uint8_t lastChip = rfid::kInvalidCardName;
        while (lastChip = Scanner->scanCard() != rfid::kInvalidCardName){}
        while (lastChip < 2 || lastChip > 4) {
            //ERROR
            Serial.println("Invalid number of players");
            //Audio
            //Scan chip to get player count
            lastChip = rfid::kInvalidCardName;
            while (lastChip = Scanner->scanCard() != rfid::kInvalidCardName){}
        }
        playerCount = lastChip;
        lastChip = -1;
    }

    void LogicPlayer::handleSelfCollision(LogicBoard* Board, int possibleMove, vector<int> &possibleMoves, int index) {    
        int color = getPlayerColor(currentPlayer);
        if (Board->currentLocations[possibleMove] == color) {
            possibleMoves.erase(possibleMoves.begin() + index);
        }
    }
} 