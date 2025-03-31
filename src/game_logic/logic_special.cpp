/// logic_special.cpp

#include "game_logic/logic_special.hpp"
#include "game_logic/logic_board.hpp"
#include "game_logic/logic_player.hpp"
#include "game_logic/logic_terminal.hpp"
#include "rfid/rfid_scanner.hpp"
#include "game_logic/logic_calculations.hpp"
#include "led_control/led.hpp"
#include <algorithm>
#include <FastLED.h>


namespace logic {
    LogicSpecial::LogicSpecial() {}

    void LogicSpecial::handleWhoops(rfid::RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves) {
        if (Scanner->lastChip == 0) {
            int opponentPawn;
            Serial.print("Select an opponent's pawn to send back to start: ");
            while (!Serial.available()) {}
            opponentPawn = readIntFromSerial();
            while (find(possibleMoves.begin(), possibleMoves.end(), opponentPawn) == possibleMoves.end()) {
                Serial.println("Invalid pawn");
                Serial.print("Select an opponent's pawn to send back to start: ");
                while (!Serial.available()) {}
                opponentPawn = readIntFromSerial();
            }
            int opponentColor = Board->currentLocations[opponentPawn];
            Board->currentLocations[Board->findNextOpenStart(opponentColor)] = opponentColor;
            Board->currentLocations[opponentPawn] = Player->getPlayerColor(Player->currentPlayer);
            Board->currentLocations[Calc->movingFrom] = 0;
            //Slide if on slide square
            int newLocation = Board->checkSlide(Player, opponentPawn);
            Serial.println("Press any key to confirm opponent's pawn has been sent back to start and your pawn has been set in their previous location.");
            while (!Serial.available()) {}
            Serial.read();
        }
    }

    void LogicSpecial::handleSeven(rfid::RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves, int movingFrom, TaskHandle_t led_task) {
        if (Scanner->lastChip == 7) {
            int color = Player->getPlayerColor(Player->currentPlayer);
            Serial.print("Place your pawn in a valid location: ");
            while (!Serial.available()) {}
            int location = readIntFromSerial();
            while (find(possibleMoves.begin(), possibleMoves.end(), location) == possibleMoves.end()) {
                Serial.println("Invalid location");
                Serial.print("Place your pawn in a valid location: ");
                while (!Serial.available()) {}
                location = readIntFromSerial();
            }
            Board->currentLocations[location] = color;
            Board->currentLocations[movingFrom] = 0;
            vTaskDelete(led_task); // turn off leds
            //Slide if on slide square
            int newLocation = Board->checkSlide(Player, location);
            int firstDistance = Calc->getDistance(Player, movingFrom, location);
            int secondDistance = 7 - firstDistance;
            if (secondDistance == 0) {
                return;
            }
            Serial.println("You have " + String(secondDistance) + " spaces left to move with your second pawn.");
            Serial.print("Possible second pawn current locations(s): ");
            for (int i = 0; i < 44; i++) {
                if (Board->currentLocations[i] == color && i != location && i != newLocation) {
                    Serial.printf("%d ", i);
                }
            }
            for (int i = 0; i < kSafetyLocations.size(); i++) {
                if (Board->currentLocations[kSafetyLocations[i]] == color && kSafetyLocations[i] != location) {
                    Serial.printf("%d ", kSafetyLocations[i]);
                }
            }
            Serial.println();
            Serial.print("Select a pawn location to move from: ");
            while (!Serial.available()) {}
            int secondPawnStart = readIntFromSerial();
            while (Board->currentLocations[secondPawnStart] != color) {
                Serial.println("Invalid pawn");
                Serial.print("Select a pawn to move: ");
                while (!Serial.available()) {}
                secondPawnStart = readIntFromSerial();
            }
            moveSecondPawn(Board, Player, secondDistance, secondPawnStart);
        }
    }

    void LogicSpecial::moveSecondPawn(LogicBoard* Board, LogicPlayer* Player, int distance, int start) { // Need to finish
        Serial.print("Move your second pawn " + String(distance) + " spaces forward. Press any key to confirm:");
        Serial.read();
        Board->currentLocations[start] = 0;
        
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
        TaskHandle_t led_task = NULL;
        led_control::indicate_moves({location}, color, location, &led_task);
        while (!Serial.available()) {}
        vTaskDelete(led_task); // turn off leds
        
        //If piece hits other piece, send other piece back to start
        if (Board->currentLocations[location] != 0) {
            Serial.print("COLLISION: Send opponent's piece back to start. Press any key to confirm: ");
            while (!Serial.available()) {}
            Serial.read();
            Board->currentLocations[Board->findNextOpenStart(Board->currentLocations[location])] = Board->currentLocations[location];
        }
        Board->currentLocations[location] = color;
        Board->currentLocations[start] = 0;
        //Slide if on slide square
        location = Board->checkSlide(Player, location);
    }

    void LogicSpecial::handleEleven(rfid::RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, vector<int> possibleMoves, int movingFrom) {
        if (Scanner->lastChip == 11) {
            int color = Player->getPlayerColor(Player->currentPlayer);
            int endLocation;
            Serial.println("Select an opponent's pawn to send back to swap with or move forward 11 spaces");
            Serial.print("Enter the oppoents's pawn location or the location you are moving to: ");
            while (!Serial.available()) {}
            endLocation = readIntFromSerial();
            while (find(possibleMoves.begin(), possibleMoves.end(), endLocation) == possibleMoves.end()) {
                Serial.println("Invalid location");
                Serial.print("Enter the oppoents's pawn location or the location you are moving to: ");
                while (!Serial.available()) {}
                endLocation = readIntFromSerial();
            }
            if (Board->currentLocations[endLocation] == 0) {
                Board->currentLocations[endLocation] = color;
                //Slide if on slide square
                endLocation = Board->checkSlide(Player, endLocation);
                Board->currentLocations[movingFrom] = 0;
            } else {
                int opponentColor = Board->currentLocations[endLocation];
                Board->currentLocations[movingFrom] = opponentColor;
                //Slide if on slide square
                int opponentPosition = Board->checkSlide(Player, movingFrom);
                Board->currentLocations[endLocation] = color;
                //Slide if on slide square
                endLocation = Board->checkSlide(Player, endLocation);
                Serial.print("Press any key to confirm opponent's pawn swapped with your pawn.");
                while (!Serial.available()) {}
                Serial.read();
            }
        }
    }
}