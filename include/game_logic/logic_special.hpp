/// logic_special.hpp
#ifndef GAME_BOARD_LOGIC_SPECIAL_HPP
#define GAME_BOARD_LOGIC_SPECIAL_HPP

#include <vector>
#include <iostream>

#include "rfid/rfid_scanner.hpp"
#include "piece_detection/piece_detection.hpp"

using namespace std;

namespace logic {
    class RfidScanner;
    class LogicBoard;
    class LogicPlayer;  
    class LogicCalculations;
    class LogicSpecial {
        public:
            LogicSpecial();
            void handleWhoops(rfid::RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves, piece_detection::PieceDetection* pieceDetection);
            void handleSeven(rfid::RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, LogicCalculations* Calc, vector<int> possibleMoves, int movingFrom, piece_detection::PieceDetection* pieceDetection);
            void moveSecondPawn(LogicBoard* Board, LogicPlayer* Player, int distance, int start, piece_detection::PieceDetection* pieceDetection);
            void handleEleven(rfid::RfidScanner* Scanner, LogicBoard* Board, LogicPlayer* Player, vector<int> possibleMoves, int movingFrom, piece_detection::PieceDetection* pieceDetection);
    };
}

#endif // GAME_BOARD_LOGIC_SPECIAL_HPP