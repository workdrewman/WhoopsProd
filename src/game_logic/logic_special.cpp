/// logic_special.cpp

#include "game_logic/logic_special.hpp"
#include "game_logic/logic_board.hpp"
#include "game_logic/logic_player.hpp"
#include "game_logic/logic_calculations.hpp"
#include "game_logic/logic_chip.hpp"
#include <algorithm>
#include <FastLED.h>


namespace logic {
    LogicSpecial::LogicSpecial() {}

    void LogicSpecial::handleWhoops(LogicChip* Chip, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves) {
        if (Chip->lastChip == 0) {
            int opponentPawn;
            Serial.print("Select an opponent's pawn to send back to start: ");
            opponentPawn = stoi(Serial.readStringUntil('\n').c_str());
            while (find(possibleMoves.begin(), possibleMoves.end(), opponentPawn) == possibleMoves.end()) {
                Serial.println("Invalid pawn");
                Serial.print("Select an opponent's pawn to send back to start: ");
                opponentPawn = stoi(Serial.readStringUntil('\n').c_str());
            }
            int opponentColor = Board->currentLocations[opponentPawn];
            Board->currentLocations[Board->findNextOpenStart(opponentColor)] = opponentColor;
            Board->currentLocations[opponentPawn] = Player->getPlayerColor(Player->currentPlayer);
            Board->currentLocations[Calc->movingFrom] = 0;
            Serial.println("Press any key to confirm opponent's pawn has been sent back to start and your pawn has been set in their previous location.");
            while (!Serial.available()) {}
            Serial.read();
        }
    }

    void LogicSpecial::handleSeven(LogicChip* Chip, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves, int movingFrom) {
        if (Chip->lastChip == 7) {
            int color = Player->getPlayerColor(Player->currentPlayer);
            Serial.print("Place your pawn in a valid location: ");
            int location = stoi(Serial.readStringUntil('\n').c_str());
            while (find(possibleMoves.begin(), possibleMoves.end(), location) == possibleMoves.end()) {
                Serial.println("Invalid location");
                Serial.print("Place your pawn in a valid location: ");
                location = stoi(Serial.readStringUntil('\n').c_str());
            }
            Board->currentLocations[location] = color;
            Board->currentLocations[movingFrom] = 0;
            int firstDistance = Calc->getDistance(Player, movingFrom, location);
            int secondDistance = 7 - firstDistance;
            Serial.println("You have " + String(secondDistance) + " spaces left to move with your second pawn.");
            Serial.print("Possible second pawn current locations(s): ");
            for (int i = 0; i < 44; i++) {
                if (Board->currentLocations[i] == color && i != location) {
                    Serial.print(i + " ");
                }
            }
            for (int i = 0; i < kSafetyLocations.size(); i++) {
                if (Board->currentLocations[kSafetyLocations[i]] == color && kSafetyLocations[i] != location) {
                    Serial.print(kSafetyLocations[i] + " ");
                }
            }
            Serial.println();
            Serial.print("Select a pawn location to move from: ");
            int secondPawnStart = stoi(Serial.readStringUntil('\n').c_str());
            while (Board->currentLocations[secondPawnStart] != color) {
                Serial.println("Invalid pawn");
                Serial.print("Select a pawn to move: ");
                secondPawnStart = stoi(Serial.readStringUntil('\n').c_str());
            }
            Serial.print("Move your second pawn " + String(secondDistance) + " spaces forward. Press any key to confirm:");
            while (!Serial.available()) {}
            Serial.read();
            Board->currentLocations[secondPawnStart] = 0;
            moveSecondPawn(Board, Player, secondDistance, secondPawnStart);
        }
    }

    void LogicSpecial::moveSecondPawn(LogicBoard* Board, LogicPlayer* Player, int distance, int start) { // Need to finish
        int color = Player->getPlayerColor(Player->currentPlayer);
        int location = start;
        for (int i = 0; i < distance; i++) {
            if (location == 43) {
                location = 0;
            }
            else if (location == 1 + 11*(color-1)) {
                location = 49 + 9*(color-1);
            }
            else if (location == 49 + 9*(color-1)) {
                location = 48 + 9*(color-1);
            }
            else if (location == 48 + 9*(color-1)) {
                location = 47 + 9*(color-1);
            }
            else if (location == 47 + 9*(color-1)) {
                location = Board->findNextOpenHome(color);
            }
            else if (location >= 0 && location < 43) {
                location++;
            }
        }

        //If piece hits other piece, send other piece back to start
        if (Board->currentLocations[location] != 0) {
            Serial.print("COLLISION: Send opponent's piece back to start. Press any key to confirm: ");
            while (!Serial.available()) {}
            Serial.read();
            Board->currentLocations[Board->findNextOpenStart(Board->currentLocations[location])] = Board->currentLocations[location];
        }
        Board->currentLocations[location] = color;
        Board->currentLocations[start] = 0;
    }

    void LogicSpecial::handleEleven(LogicChip* Chip, LogicBoard* Board, LogicPlayer* Player, vector<int> possibleMoves, int movingFrom) {
        if (Chip->lastChip == 11) {
            int color = Player->getPlayerColor(Player->currentPlayer);
            int endLocation;
            Serial.println("Select an opponent's pawn to send back to swap with or move forward 11 spaces");
            Serial.print("Enter the oppoents's pawn location or the location you are moving to: ");
            endLocation = stoi(Serial.readStringUntil('\n').c_str());
            while (find(possibleMoves.begin(), possibleMoves.end(), endLocation) == possibleMoves.end()) {
                Serial.println("Invalid location");
                Serial.print("Enter the oppoents's pawn location or the location you are moving to: ");
                endLocation = stoi(Serial.readStringUntil('\n').c_str());
            }
            if (Board->currentLocations[endLocation] == 0) {
                Board->currentLocations[endLocation] = color;
            } else {
                int opponentColor = Board->currentLocations[endLocation];
                Board->currentLocations[movingFrom] = opponentColor;
                Board->currentLocations[endLocation] = color;
                Serial.print("Press any key to confirm opponent's pawn swapped with your pawn.");
                while (!Serial.available()) {}
                Serial.read();
            }
        }
    }
}