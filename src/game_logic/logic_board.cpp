/// logic_board.cpp

#include "game_logic/logic_board.hpp"
#include "game_logic/logic_player.hpp"
#include "game_logic/logic_terminal.hpp"
#include <algorithm>
#include <FastLED.h>

namespace logic {

    vector<int> LogicBoard::findOtherPawnLocations(LogicPlayer* Player, int currentLocation) {  //Doesn't find pawns in home or start, but does find pawns in safety zone
        vector<int> otherPawnLocations;
        int color = Player->getPlayerColor(Player->currentPlayer);
        for (int i = 0; i < 80; i++) {
            if (currentLocations[i] == color && i != currentLocation) {
                otherPawnLocations.push_back(i);
            }
        }
        return otherPawnLocations;
    }

    void LogicBoard::movePiece(int startIndex, int endIndex) {
        currentLocations[endIndex] = currentLocations[startIndex];
        currentLocations[startIndex] = 0;
    }

    bool LogicBoard::isCollision(int location, int playerColor) {
        return currentLocations[location] != 0 &&
            currentLocations[location] != playerColor;
    }

    bool LogicBoard::checkWinCondition(LogicPlayer* Player) {
        int color = Player->getPlayerColor(Player->currentPlayer);

        //Check if all pieces are in home
        switch (color) {
            //Yellow
            case 1:
                for (int i = 44; i < 47; i++) {
                    if (currentLocations[i] != 1) {
                        return false;
                    }
                }
                break;
            //Green
            case 2:
                for (int i = 53; i < 56; i++) {
                    if (currentLocations[i] != 2) {
                        return false;
                    }
                }
                break;
            //Red
            case 3:
                for (int i = 62; i < 65; i++) {
                    if (currentLocations[i] != 3) {
                        return false;
                    }
                }
                break;
            //Blue
            case 4:
                for (int i = 71; i < 74; i++) {
                    if (currentLocations[i] != 4) {
                        return false;
                    }
                }
                break;
        }
        return true;
    }

    int LogicBoard::findNextOpenHome(int color) {
        switch (color) {
            case 1:     //Yellow
                for (int i = 44; i < 47; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
            case 2:     //Green
                for (int i = 53; i < 56; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
            case 3:     //Red
                for (int i = 62; i < 65; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
            case 4:     //Blue
                for (int i = 71; i < 74; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
        }
        return -1;
    }

    int LogicBoard::findNextOpenStart(int color) {
        switch (color) {
            case 1:     //Yellow
                for (int i = 50; i < 53; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
            case 2:     //Green
                for (int i = 59; i < 62; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
            case 3:     //Red
                for (int i = 68; i < 71; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
            case 4:     //Blue
                for (int i = 77; i < 80; i++) {
                    if (currentLocations[i] == 0) {
                        return i;
                    }
                }
                break;
        }
        return -1;
    }

    //Returns the new location of the piece
    int LogicBoard::checkSlide(LogicPlayer* Player, int location) {
        if (find(kSlideStartLocations.begin(), kSlideStartLocations.end(), location) != kSlideStartLocations.end()) {
            //Can't slide on your own color
            int color = currentLocations[location];
            if (color == 1 && (location == 0 || location == 6)) {  //Yellow
                return location;
            }
            else if (color == 2 && (location == 11 || location == 17)) {  //Green
                return location;
            }
            else if (color == 3 && (location == 22 || location == 28)) {  //Red
                return location;
            }
            else if (color == 4 && (location == 33 || location == 39)) {  //Blue
                return location;
            }

            //Otherwise slide
            int slideIndex= find(kSlideStartLocations.begin(), kSlideStartLocations.end(), location) - kSlideStartLocations.begin();
            Serial.println("Move the piece to the end of the slide (location " + String(kSlideEndLocations[slideIndex]) + ") and send any pawns you collide with back to their Start.");
            for (int i = kSlideStartLocations[slideIndex] + 1; i <= kSlideEndLocations[slideIndex]; i++) {
                if (currentLocations[i] != 0) {
                    currentLocations[findNextOpenStart(currentLocations[i])] = currentLocations[i];
                    currentLocations[i] = 0;
                }
            }
            currentLocations[kSlideEndLocations[slideIndex]] = currentLocations[location];
            currentLocations[location] = 0;
            return slideIndex;
        }
        return location;
    }

    bool LogicBoard::allPiecesPlaced() {
        int count = 0;
        for (int i = 0; i < 40; i++) {
            if (currentLocations[i]) {
                count++;
            }
        }
        return count == MAX_PIECES;
    }

    bool LogicBoard::allPiecesOnStart(LogicPlayer* Player, LogicTerminal* Terminal) {
        int numberOfFilledAreas = 0;
        //Check if 50, 51, 52 occupied
        //If true increment numberOfFilledAreas

        //Check if 59, 60, 61 occupied
        //If true increment numberOfFilledAreas

        //Check if 68, 69, 70 occupied
        //If true increment numberOfFilledAreas

        //Check if 77, 78, 79 occupied
        //If true increment numberOfFilledAreas

        //For terminal testing
        numberOfFilledAreas = Terminal->t_setStartAreas(this, Player);

        if (numberOfFilledAreas >= Player->getPlayerCount()) {
            return true;
        } else {
            return false;
        }
    }
}