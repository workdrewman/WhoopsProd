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

    void LogicPlayer::setPlayerColor(int movingFrom, int color, int player) {
        if (playerColors[player] != 0) {
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
    }

    int LogicPlayer::getPlayerCount() {
        return playerCount;
    }

    void LogicPlayer::setPlayerCount(rfid::RfidScanner* Scanner, LogicTerminal* Terminal) {
        //Scan chip to get player count
        Serial.println("Get number of players: ");
        //Audio
        uint8_t lastChip = rfid::kInvalidCardName;
        while (lastChip == rfid::kInvalidCardName){
            lastChip = Scanner->scanCard();
        }
        while (lastChip < 2 || lastChip > 4) {
            //ERROR
            Serial.println("Invalid number of players");
            //Audio
            //Scan chip to get player count
            lastChip = rfid::kInvalidCardName;
            while (lastChip == rfid::kInvalidCardName)
            {
                lastChip = Scanner->scanCard();
            }
        }
        playerCount = lastChip;
        lastChip = -1;
    }

    void LogicPlayer::handleSelfCollision(LogicBoard* Board, int possibleMove, vector<int> &possibleMoves, int index, int chip, int movingFrom) {    
        int color = getPlayerColor(currentPlayer);
        if (Board->currentLocations[possibleMove] == color) {
            possibleMoves.erase(possibleMoves.begin() + index);
        }

        //If possible move is a slide start, check if the player has any other pawns in that slide
        //But don't check slides on your own side of the board
        if (color == 1 && (possibleMove == 0 || possibleMove == 6)) {return;} //Yellow
        else if (color == 2 && (possibleMove == 11 || possibleMove == 17)) {return;} //Green
        else if (color == 3 && (possibleMove == 22 || possibleMove == 28)) {return;} //Red
        else if (color == 4 && (possibleMove == 33 || possibleMove == 39)) {return;} //Blue

        if (find(kSlideStartLocations.begin(), kSlideStartLocations.end(), possibleMove) != kSlideStartLocations.end()) {
            for (int i = possibleMove; i <= possibleMove + 3; i++) {
                if (chip == 10 && i == possibleMove + 1 && possibleMove + 1 == movingFrom) {
                    continue; //Don't check the second location of the slide on a 10 for backwards 1
                }
                if (Board->currentLocations[i] == color) {
                    possibleMoves.erase(possibleMoves.begin() + index);
                    break;
                }
            }
        }
    }
} 