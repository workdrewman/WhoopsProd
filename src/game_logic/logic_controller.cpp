#include "game_logic/logic_controller.hpp"

#include <vector> // for std::vector
#include <iostream> // for std::cout and std::endl
#include <algorithm> // for std::find
#include <FastLED.h> // for LEDs
#include "led_control/led.hpp" // for helper functions
#include "piece_detection/piece_detection.hpp" // for PieceDetection

using namespace std;

namespace logic
{
    // //No chip yet: -1, Whoops!: 0, Other chips: 1-12
    // int lastChip = -1;
    // //Yellow: 1, Green: 2, Red: 3, Blue: 4
    // int player1Color = 0;
    // int player2Color = 0;
    // int player3Color = 0;
    // int player4Color = 0;

    // //Locations
    // //Map
    // // 0-43: Around the board starting from Yellow
    // // 44-46: Yellow's Home
    // // 47-49: Yellow's Safety Zone
    // // 50-52: Yellow's Start
    // // 53-55: Green's Home
    // // 56-58: Green's Safety Zone
    // // 59-61: Green's Start
    // // 62-64: Red's Home
    // // 65-67: Red's Safety Zone
    // // 68-70: Red's Start
    // // 71-73: Blue's Home
    // // 74-76: Blue's Safety Zone
    // // 77-79: Blue's Start

    LogicController::LogicController()
    : Player(), Board(), Special(), Calc(), Terminal(), Scanner(), pieceDetection{6} {
    }  

    void LogicController::startGame() {
        // Setup the piece detection with correct modes
        pieceDetection.initMCP23017();

        // Launch piece detection task
        // Create the FreeRTOS task
        xTaskCreate(
            [](void* pvParameters) {
            auto* pieceDetectionPtr = static_cast<decltype(this->pieceDetection)*>(pvParameters);
            while (true) {
                pieceDetectionPtr->readMCPInputs();
                vTaskDelay(pdMS_TO_TICKS(50));
            }
            },   // Task function
            "ReadMCPInputs",     // Name of the task
            2048,                // Stack size (in words, not bytes)
            &pieceDetection,     // Task input parameter
            1,                   // Priority of the task
            NULL                 // Task handle
        );


        Serial.println("Setup Complete");
        Player.currentPlayer = 0;
        Player.setPlayerCount(&Scanner, &Terminal);
        Serial.println("Player count: " + String(Player.getPlayerCount()));
        Serial.println();

        //code to ensure setup is done correctly
        Serial.println("Please place pieces on start locations");
        led_control::showStartPositions(Player.getPlayerCount());
        while (!Board.allPiecesOnStart(&Player, &Terminal, &pieceDetection)) {vTaskDelay(pdMS_TO_TICKS(500));};
        led_control::stopStartPositions();
        Serial.println("All pieces placed on start locations");
        Serial.println("Game starting...");

        while (!Board.checkWinCondition(&Player)){
            led_control::showCorrectPositions(&Board);
            if (pieceDetection.hasChangedSensor()) {
                // dump the changes... they can figure it out
                pieceDetection.getChangedSensors();
            }
            takeTurn();
        }
        Serial.println("Player " + String(Player.currentPlayer + 1) + " wins!");
        led_control::showWinner(Player.currentPlayer + 1);
    }

    void LogicController::takeTurn() {
        Serial.println("Player " + String(Player.currentPlayer + 1) + "'s turn");
        Serial.println();

        // //Flash LEDs for current player pieces
        led_control::showPlayerPositions(Player.currentPlayer+1, Board);

        //Scan chip
        Serial.println("Draw and scan a chip");
        
        uint8_t scan_val = rfid::kInvalidCardName;
        while (scan_val == rfid::kInvalidCardName)
        {
            scan_val = Scanner.scanCard();
        }        
        Scanner.lastChip = scan_val;
        led_control::stopPlayerPositions();
        Terminal.t_displayChipInstructions(&Scanner);
        
        // Stop showing where all pieces should be
        FastLED.clear();
        FastLED.show();

        //Pick up a piece
        Terminal.t_selectPiece(&Board, &Player, &Calc, Scanner.lastChip, &pieceDetection);

        //Light leds of possible moves
        vector<int> possibleMoves = Calc.findPossibleMoves(&Board, &Player, Calc.movingFrom, Scanner.lastChip);
        //If there are no possible moves, go to next player
        if (possibleMoves.size() == 0) {
            Serial.println("No possible moves");
            vTaskDelay(pdMS_TO_TICKS(2000)); // wait for a bit before going to next player
            nextPlayer();
            return;
        }

        Serial.print("Possible moves: ");
        for (int i = 0; i < possibleMoves.size(); i++) {
            Serial.printf("%d ", possibleMoves[i]);
        }
        Serial.println();
        
        // flash LEDs at potential move locations
        led_control::indicate_moves(possibleMoves, Player.getPlayerColor(Player.currentPlayer), Calc.movingFrom);
        
        //handle whoops, 7s, and 11s
        Special.handleWhoops(&Scanner, &Board, &Player, &Calc, possibleMoves, &pieceDetection);
        Special.handleSeven(&Scanner, &Board, &Player, &Calc, possibleMoves, Calc.movingFrom, &pieceDetection);
        Special.handleEleven(&Scanner, &Board, &Player, possibleMoves, Calc.movingFrom, &pieceDetection);

        //Place piece on new location
        int newLocation;
        if (!(Scanner.lastChip == 0 || Scanner.lastChip == 7 || Scanner.lastChip == 11)) {
            Serial.print("Select a location to move to: ");
            while (!pieceDetection.hasChangedSensor()) {} // wait until player chooses a piece
            newLocation = piece_detection::kSensorMap.at(pieceDetection.getChangedSensors().at(0));

            while (find(possibleMoves.begin(), possibleMoves.end(), newLocation) == possibleMoves.end()) {
                Serial.print("Invalid location, please select a valid location: ");
                while (!pieceDetection.hasChangedSensor()) {} // wait until player chooses a piece
                newLocation = piece_detection::kSensorMap.at(pieceDetection.getChangedSensors().at(0));
            }
            led_control::stopIndicateMoves();
            Serial.printf("\nMoving player %d's piece %d ----> %d\n", Player.currentPlayer + 1, Calc.movingFrom, newLocation);

            //If piece hits other piece, send other piece back to start
            if (Board.currentLocations[newLocation] != 0) {
                Serial.print("COLLISION: Send opponent's piece back to start.");
                Board.currentLocations[Board.findNextOpenStart(Board.currentLocations[newLocation])] = Board.currentLocations[newLocation];
            }
            Board.currentLocations[newLocation] = Board.currentLocations[Calc.movingFrom];
            Board.currentLocations[Calc.movingFrom] = 0;

            //Slide if on slide square
            newLocation = Board.checkSlide(&Player, newLocation, &pieceDetection);
        } else if (Scanner.lastChip == 0 || Scanner.lastChip == 11) {
            led_control::stopIndicateMoves();
        }
        
        if (Board.checkWinCondition(&Player)) {
            return;
        } else {
            nextPlayer();
            return;
        }
    }

    //Next player, 4 players in total
    void LogicController::nextPlayer() {
        Player.currentPlayer = (Player.currentPlayer + 1) % Player.getPlayerCount();
    }
}
