/// logic_terminal.cpp

#include "game_logic/logic_terminal.hpp"
#include "game_logic/logic_board.hpp"
#include "game_logic/logic_player.hpp"
#include "rfid/rfid_scanner.hpp"
#include "game_logic/logic_calculations.hpp"
#include "game_logic/logic_special.hpp"
#include "led_control/led.hpp"
#include <FastLED.h>
#include <algorithm>

using namespace std;

namespace logic {
    LogicTerminal::LogicTerminal() {}
    
    int LogicTerminal::t_GetChip() {
        Serial.print("Enter chip number: ");
        int chip = stoi(Serial.readStringUntil('\n').c_str());
        return chip;
    }

    int LogicTerminal::t_setStartAreas(LogicBoard* Board, LogicPlayer* Player) {
        Board->currentLocations[50] = 1;
        Board->currentLocations[51] = 1;
        Board->currentLocations[52] = 1;
        Board->currentLocations[59] = 2;
        Board->currentLocations[60] = 2;
        Board->currentLocations[61] = 2;
        if (Player->getPlayerCount() >= 3) {
            Board->currentLocations[68] = 3;
            Board->currentLocations[69] = 3;
            Board->currentLocations[70] = 3;
        }
        if (Player->getPlayerCount() == 4) {
            Board->currentLocations[77] = 4;
            Board->currentLocations[78] = 4;
            Board->currentLocations[79] = 4;
        }

        //Set colors too lol
        Player->playerColors[0] = 1;
        Player->playerColors[1] = 2;
        if (Player->getPlayerCount() >= 3) {
            Player->playerColors[2] = 3;
        }
        if (Player->getPlayerCount() == 4) {
            Player->playerColors[3] = 4;
        }

        Board->lastLocations = Board->currentLocations;
        return Player->getPlayerCount();
    }

    void LogicTerminal::t_displayChipInstructions(rfid::RfidScanner* Scanner) {
        switch (Scanner->lastChip) {
            case 0:
                Serial.println("Whoops! Take any one pawn from Start and move it directly to a square occupied by any opponent's pawn, sending that pawn back to its own Start. A Whoops! chip cannot be used on an opponent's pawn in a Safety Zone or at the Home base. If there are no pawns on the player's Start, or no opponent's pawns on any space that can be moved to, the turn ends.");
                break;
            case 1:
                Serial.println("Either move a pawn from Start or move a pawn 1 space forward.");
                break;
            case 2:
                Serial.println("Either move a pawn from Start or move a pawn 2 spaces forward."); //Maybe add draw again
                break;
            case 3:
                Serial.println("Move a pawn 3 spaces forward.");
                break;
            case 4:
                Serial.println("Move a pawn 4 spaces backward.");
                break;
            case 5:
                Serial.println("Move a pawn 5 spaces forward.");
                break;
            case 7:
                Serial.println("Move one pawn 7 spaces forward, or split the 7 spaces between two pawns (such as 4 spaces for one pawn and 3 for another).");
                break;
            case 8:
                Serial.println("Move a pawn 8 spaces forward.");
                break;
            case 10:
                Serial.println("Move a pawn 10 spaces forward or 1 space backward. If none of a player's pawns can move forward 10 spaces, then one pawn must move back 1 space.");
                break;
            case 11:
                Serial.println("Move 11 spaces forward, or switch the places of one of the player's own pawns and an opponent's pawn. A player who cannot move 11 spaces is not forced to switch and instead can end their turn. An 11 cannot be used to switch a pawn that is in a Safety Zone.");
                break;
            case 12:
                Serial.println("Move a pawn 12 spaces forward.");
                break;
        }
        Serial.println();
    }

    void LogicTerminal::t_selectPiece(LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, int chip) {
        int color = Player->getPlayerColor(Player->currentPlayer);
        Serial.print("Player " + String(Player->currentPlayer + 1) + "'s pieces: ");
        vector<int> possibleMoves, validPieces;
        for (int i = 0; i < kBoardSize; i++) {
            if (Board->currentLocations[i] == color) {
                possibleMoves = Calc->findPossibleMoves(Board, Player, i, chip);
                if (possibleMoves.size() != 0) {
                    Serial.printf("%d ",i);
                    validPieces.push_back(i);
                }
            }
        }
        Serial.println();

        if (validPieces.size() == 0) {
            Calc->movingFrom = -1;
            return;
        }
        int location;
        Serial.print("Select a location to move " + String(Player->currentPlayer + 1) + "'s piece from: ");
        location = readIntFromSerial();
        while (Board->currentLocations[location] != color) {
            Serial.println("Invalid piece");
            Serial.print("Select a piece to move: ");
            location = readIntFromSerial();
        }
        Calc->movingFrom = location;
        //setPlayerColor();
    }

    int readIntFromSerial() {
        while (true) {            
            // Wait for user input
            while (!Serial.available()) {
                delay(10);  // Avoids busy-waiting
            }
    
            String input = Serial.readStringUntil('\n');  // Read the input
            input.trim();  // Remove leading/trailing spaces or newlines
    
            if (input.length() == 0) {  // Check if input is empty
                Serial.println("Error: No input received. Try again.");
                continue;
            }
    
            char *end;
            long number = strtol(input.c_str(), &end, 10);  // Convert safely
    
            if (*end != '\0') {  // Check if the input was a valid number
                Serial.println("Error: Invalid input. Please enter a valid integer.");
                continue;
            }
    
            return static_cast<int>(number);  // Return the valid integer
        }
    }
    
}