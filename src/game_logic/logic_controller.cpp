#include "game_logic/logic_controller.hpp"

#include <vector> // for std::vector
#include <iostream> // for std::cout and std::endl
#include <algorithm> // for std::find
#include <FastLED.h> // for LEDs
#include "led_control/led.hpp" // for helper functions

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
    : Player(), Board(), Special(), Calc(), Terminal(), Scanner() {
    }

    void LogicController::startGame() {
        Serial.println("Setup Complete");
        Serial.println("Press any key to start game");
        while (!Serial.available()) {}
        Serial.read();
        Serial.println("Game Started");
        Player.currentPlayer = 0;
        Player.setPlayerCount(&Scanner, &Terminal);
        Serial.println("Player count: " + String(Player.getPlayerCount()));
        Serial.println();

        //code to ensure setup is done correctly
        Serial.println("Please place pieces on start locations");
        while (!Board.allPiecesOnStart(&Player, &Terminal)) {};

        while (!Board.checkWinCondition(&Player)){
            takeTurn();
        }
        Serial.println("Player " + String(Player.currentPlayer + 1) + " wins!");

    }

    void LogicController::takeTurn() {
        Serial.println("Player " + String(Player.currentPlayer + 1) + "'s turn");
        Serial.println();
        //Scan chip
        Serial.println("Draw and scan a chip");
        Scanner.lastChip = Scanner.scanCard();
        Terminal.t_displayChipInstructions(&Scanner);

        //Pick up a piece
        Terminal.t_selectPiece(&Board, &Player, &Calc, Scanner.lastChip);

        //Light leds of possible moves
        vector<int> possibleMoves = Calc.findPossibleMoves(&Board, &Player, Calc.movingFrom, Scanner.lastChip);
        //If there are no possible moves, go to next player
        if (possibleMoves.size() == 0) {
            Serial.println("No possible moves");
            nextPlayer();
            return;
        }

        led_control::indicate_possible_moves(Calc.movingFrom, possibleMoves, Player.getPlayerColor(Player.currentPlayer));

        Serial.print("Possible moves: ");
        for (int i = 0; i < possibleMoves.size(); i++) {
            Serial.printf("%d ", possibleMoves[i]);
        }
        Serial.println();

        // set LEDs

        //handle whoops, 7s, and 11s
        Special.handleWhoops(&Scanner, &Board, &Player, &Calc, possibleMoves);
        Special.handleSeven(&Scanner, &Board, &Player, &Calc, possibleMoves, Calc.movingFrom);
        Special.handleEleven(&Scanner, &Board, &Player, possibleMoves, Calc.movingFrom);
        //Place piece on new location
        int newLocation;
        if (!(Scanner.lastChip == 0 || Scanner.lastChip == 7 || Scanner.lastChip == 11)) {
            Serial.print("Select a location to move to: ");
            newLocation = readIntFromSerial();
            while (find(possibleMoves.begin(), possibleMoves.end(), newLocation) == possibleMoves.end()) {
                Serial.print("Invalid location, please select a valid location: ");
                newLocation = readIntFromSerial();
            }
            Serial.printf("\nMoving player %d's piece %d ----> %d\n", Player.currentPlayer + 1, Calc.movingFrom, newLocation);
            //If piece hits other piece, send other piece back to start
            if (Board.currentLocations[newLocation] != 0) {
                Serial.print("COLLISION: Send opponent's piece back to start. Press any key to confirm: ");
                while (!Serial.available()) {}
                Serial.read();
                Board.currentLocations[Board.findNextOpenStart(Board.currentLocations[newLocation])] = Board.currentLocations[newLocation];
            }
            Board.currentLocations[newLocation] = Board.currentLocations[Calc.movingFrom];
            Board.currentLocations[Calc.movingFrom] = 0;

            //Slide if on slide square
            newLocation = Board.checkSlide(&Player, newLocation);
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