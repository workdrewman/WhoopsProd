#include "../../include/game_logic/logic_controller.hpp"

#include <vector> // for std::vector
#include <iostream> // for std::cout and std::endl
#include <algorithm> // for std::find
#include <FastLED.h>

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
    : Player(), Board(), Chip(), Special(), Calc(), Terminal() {}

    void LogicController::startGame() {
        Serial.println("Setup Complete");
        Serial.println("Press any key to start game");
        while (!Serial.available()) {}
        Serial.read();
        Serial.println("Game Started");
        Player.currentPlayer = 0;
        Player.setPlayerCount(&Chip, &Terminal);
        Serial.println("Player count: " + String(Player.getPlayerCount()));
        Serial.println();

        //code to ensure setup is done correctly
        Serial.println("Please place pieces on start locations");
        while (!Board.allPiecesOnStart(&Player, &Terminal)) {};

        takeTurn();
    }

    void LogicController::takeTurn() {
        Serial.println("Player " + String(Player.currentPlayer + 1) + "'s turn");
        Serial.println();
        //Scan chip
        Serial.println("Draw and scan a chip");
        Chip.lastChip = Chip.waitForChip(&Terminal);
        Terminal.t_displayChipInstructions(&Chip);

        //Display pieces
        Terminal.t_whereAreMyPieces(&Board, &Player);

        //Pick up a piece
        Terminal.t_selectPiece(&Board, &Player, &Calc);

        //Light leds of possible moves
        vector<int> possibleMoves = Calc.findPossibleMoves(&Board, &Player, Calc.movingFrom, Chip.lastChip);
        //If there are no possible moves, go to next player
        if (possibleMoves.size() == 0) {
            Serial.println("No possible moves");
            nextPlayer();
            takeTurn();
        }
        Serial.print("Possible moves: ");
        for (int i = 0; i < possibleMoves.size(); i++) {
            Serial.print(possibleMoves[i] + " ");
        }
        Serial.println();

        //handle whoops, 7s, and 11s
        Special.handleWhoops(&Chip, &Board, &Player, &Calc, possibleMoves);
        Special.handleSeven(&Chip, &Board, &Player, &Calc, possibleMoves, Calc.movingFrom);
        Special.handleEleven(&Chip, &Board, &Player, possibleMoves, Calc.movingFrom);
        //Place piece on new location
        int newLocation;
        if (!(Chip.lastChip == 0 || Chip.lastChip == 7 || Chip.lastChip == 11)) {
            Serial.print("Select a location to move to: ");
            newLocation = int(Serial.readStringUntil('\n'));
            while (find(possibleMoves.begin(), possibleMoves.end(), newLocation) == possibleMoves.end()) {
                Serial.print("Invalid location, please select a valid location: ");
                newLocation = int(Serial.readStringUntil('\n'));
            }
            //If piece hits other piece, send other piece back to start
            if (Board.currentLocations[newLocation] != 0) {
                Serial.print("COLLISION: Send opponent's piece back to start. Press any key to confirm: ");
                while (!Serial.available()) {}
                Serial.read();
                Board.currentLocations[Board.findNextOpenStart(Board.currentLocations[newLocation])] = Board.currentLocations[newLocation];
            }
            Board.currentLocations[newLocation] = Board.currentLocations[Calc.movingFrom];
            Board.currentLocations[Calc.movingFrom] = 0;
        }

        //Slide if on slide square
        Board.checkSlide(&Player, newLocation);
        
        if (Board.checkWinCondition(&Player)) {
            Serial.println("Player " + String(Player.currentPlayer + 1) + " wins!");
        } else {
            nextPlayer();
            takeTurn();
        }
    }

    //Next player, 4 players in total
    void LogicController::nextPlayer() {
        Player.currentPlayer = (Player.currentPlayer + 1) % Player.getPlayerCount();
    }
}