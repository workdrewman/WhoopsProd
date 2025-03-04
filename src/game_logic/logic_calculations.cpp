/// logic_calculations.cpp
#include "game_logic/logic_calculations.hpp"
#include "game_logic/logic_board.hpp"
#include "game_logic/logic_player.hpp"

namespace logic {
    LogicCalculations::LogicCalculations() : movingFrom(-1) {}

    vector<int> LogicCalculations::findPossibleMoves(LogicBoard* Board, LogicPlayer* Player, int selectedPawn, int distance) {
        vector<int> possibleMoves;
        int color = Player->getPlayerColor(Player->currentPlayer);
        switch (distance) {
            case 0: //Whoops!
                if (selectedPawn >= (50 + (9*(color-1))) && selectedPawn <= 52 + (9*(color-1))) {   //If pawn is in start
                    for (int i = 0; i < 44; i++) {
                        if(Board->currentLocations[i] != color && Board->currentLocations[i] != 0) {
                            possibleMoves.push_back(i);
                        }
                    }
                }
                break;
            case 1:
                if (selectedPawn >= (50 + (9*(color-1))) && selectedPawn <= 52 + (9*(color-1))) {   //If pawn is in start
                    possibleMoves.push_back(3 + (11*(color-1)));
                } 
                else if (selectedPawn == (1 + (11*(color-1)))) {  //If pawn can move to safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if ((selectedPawn == (48 + 9*(color - 1))) || (selectedPawn == (49 + 9*(color - 1)))) {    //Pawn is in safety zone
                    possibleMoves.push_back(selectedPawn - 1);
                }
                else if (selectedPawn == (47 + 9*(color-1))) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 1 space forwards
                    possibleMoves.push_back((selectedPawn + 1)%44);
                }
                break;
            case 2:
                if (selectedPawn >= (50 + (9*(color-1))) && selectedPawn <= 52 + (9*(color-1))) {   //If pawn is in start
                    possibleMoves.push_back(4 + (11*(color-1)));
                } 
                else if (selectedPawn == (1 + (11*(color-1)))) {  //If pawn is one away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                }
                else if (selectedPawn == (0 + (11*(color-1)))) {  //If pawn is two away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if (selectedPawn == (49 + 9*(color-1))) {    //Pawn is in safety zone
                    possibleMoves.push_back(selectedPawn - 2);
                }
                else if (selectedPawn == (48 + 9*(color-1))) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if (selectedPawn == (47 + 9*(color-1))) {    //Pawn too close to home
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 2 spaces forwards
                    possibleMoves.push_back((selectedPawn + 2)%44);
                }
                break;
            case 3:
                if (selectedPawn == (1 + (11*(color-1)))) {  //If pawn is one away from safety zone
                    possibleMoves.push_back(47 + (9*(color-1)));
                }
                else if (selectedPawn == (0 + (11*(color-1)))) {  //If pawn is two away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                }
                else if (selectedPawn == ((43 + (11*(color-1)))%44)) {  //If pawn is three away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if (selectedPawn == (49 + 9*(color-1))) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if ((selectedPawn >= (47 + 9*(color-1))) && (selectedPawn <= (48 + 9*(color-1)))) {    //Pawn too close to home
                    break;
                }
                else if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 3 spaces forwards
                    possibleMoves.push_back((selectedPawn + 3)%44);
                }
                break;
            case 4:
                if (selectedPawn == (47 + (9*(color-1)))) {  //If pawn is in safety zone near home
                    possibleMoves.push_back(0 + (11*(color-1)));
                }
                else if (selectedPawn == (48 + (9*(color-1)))) {  //If pawn is in saftey zone middle
                    possibleMoves.push_back((43 + (11*(color-1)))%44);
                }
                else if (selectedPawn == (49 + (9*(color-1)))) {  //If pawn is in safety zone near exit
                    possibleMoves.push_back((42 + (11*(color-1)))%44);
                }
                else if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 4 spaces backwards
                    int newLocation = selectedPawn - 4;
                    if (newLocation < 0) {
                        newLocation += 44;
                    }
                    possibleMoves.push_back(newLocation);
                }
                break;
            case 5:
                if (selectedPawn == ((43 + (11*(color-1))))%44) {  //If pawn is three away from safety zone
                    possibleMoves.push_back(47 + (9*(color-1)));
                }
                else if (selectedPawn == ((42 + (11*(color-1))))%44) {  //If pawn is four away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                }
                else if (selectedPawn == ((41 + (11*(color-1)))%44)) {  //If pawn is five away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if (selectedPawn == (0 + 11*(color-1))) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if (((selectedPawn >= (47 + 9*(color-1))) && (selectedPawn <= (49 + 9*(color-1)))) || (selectedPawn == (1 + 11*(color-1)))) {    //Pawn too close to home
                    break;
                }
                else if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 5 spaces forwards
                    possibleMoves.push_back((selectedPawn + 5)%44);
                }
                break;
            case 7:
                if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else if (selectedPawn == (47 + 9*(color-1))) {  //One away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    checkSecondPawnDistance(Board, Player, 1, possibleMoves);
                }
                else if (selectedPawn == (48 + 9*(color-1))) {  //Two away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(selectedPawn - 1);
                    checkSecondPawnDistance(Board, Player, 2, possibleMoves);
                }
                else if (selectedPawn == (49 + 9*(color-1))) {  //Three away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(selectedPawn - 1);
                    possibleMoves.push_back(selectedPawn - 2);
                    checkSecondPawnDistance(Board, Player, 3, possibleMoves);
                }
                else if (selectedPawn == (1 + 11*(color-1))) {  //Four away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(47 + 9*(color-1));
                    possibleMoves.push_back(48 + 9*(color-1));
                    possibleMoves.push_back(49 + 9*(color-1));
                    checkSecondPawnDistance(Board, Player, 4, possibleMoves);
                }
                else if (selectedPawn == (0 + 11*(color-1))) {  //Five away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(47 + 9*(color-1));
                    possibleMoves.push_back(48 + 9*(color-1));
                    possibleMoves.push_back(49 + 9*(color-1));
                    possibleMoves.push_back(1 + 11*(color-1));
                    checkSecondPawnDistance(Board, Player, 5, possibleMoves);
                }
                else if (selectedPawn == ((43 + 11*(color-1))%44)) {    //Six away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(47 + 9*(color-1));
                    possibleMoves.push_back(48 + 9*(color-1));
                    possibleMoves.push_back(49 + 9*(color-1));
                    possibleMoves.push_back(1 + 11*(color-1));
                    possibleMoves.push_back(0 + 11*(color-1));
                    checkSecondPawnDistance(Board, Player, 6, possibleMoves);
                }
                else if (selectedPawn == ((42 + 11*(color-1))%44)) {    //Seven away from home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(47 + 9*(color-1));
                    possibleMoves.push_back(48 + 9*(color-1));
                    possibleMoves.push_back(49 + 9*(color-1));
                    possibleMoves.push_back(1 + 11*(color-1));
                    possibleMoves.push_back(0 + 11*(color-1));
                    possibleMoves.push_back((43 + 11*(color-1))%44);
                    checkSecondPawnDistance(Board, Player, 7, possibleMoves);
                }
                else {
                    possibleMoves.push_back((selectedPawn + 1)%44);
                    possibleMoves.push_back((selectedPawn + 2)%44);
                    possibleMoves.push_back((selectedPawn + 3)%44);
                    possibleMoves.push_back((selectedPawn + 4)%44);
                    possibleMoves.push_back((selectedPawn + 5)%44);
                    possibleMoves.push_back((selectedPawn + 6)%44);
                    possibleMoves.push_back((selectedPawn + 7)%44);
                    checkSecondPawnDistance(Board, Player, 7, possibleMoves);
                }
                //Need to check if second pawn can move the left over amount and set leftOverSpaces
                break;
            case 8:
                if (selectedPawn == ((40 + (11*(color-1))))%44) {  //If pawn is six away from safety zone
                    possibleMoves.push_back(47 + (9*(color-1)));
                }
                else if (selectedPawn == ((39 + (11*(color-1))))%44) {  //If pawn is seven away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                }
                else if (selectedPawn == ((38 + (11*(color-1)))%44)) {  //If pawn is eight away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if (selectedPawn == ((41 + 11*(color-1))%44)) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if (((selectedPawn >= (47 + 9*(color-1))) && (selectedPawn <= (49 + 9*(color-1)))) || (selectedPawn == (0 + 11*(color-1))) || (selectedPawn == (1 + 11*(color-1))) || ((selectedPawn >= ((42 + 11*(color-1))%44)) && (selectedPawn <= ((43 + 11*(color-1))%44)))) {    //Pawn too close to home
                    break;
                }
                else if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 8 spaces forwards
                    possibleMoves.push_back((selectedPawn + 8)%44);
                }
                break;
            case 10:
                if (selectedPawn == ((38 + (11*(color-1))))%44) {  //If pawn is eight away from safety zone
                    possibleMoves.push_back(47 + (9*(color-1)));
                    possibleMoves.push_back(selectedPawn - 1);
                }
                else if (selectedPawn == ((37 + (11*(color-1))))%44) {  //If pawn is nine away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                    possibleMoves.push_back(selectedPawn - 1);
                }
                else if (selectedPawn == ((36 + (11*(color-1)))%44)) {  //If pawn is ten away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                    possibleMoves.push_back(selectedPawn - 1);
                }
                else if (selectedPawn == ((39 + 11*(color-1))%44)) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                    possibleMoves.push_back(selectedPawn - 1);
                }
                else if ((selectedPawn >= (47 + 9*(color-1))) && (selectedPawn <= (48 + 9*(color-1)))) {    //Pawn in safety zone
                    possibleMoves.push_back(selectedPawn - 1);
                }
                else if (selectedPawn == (49 + 9*(color-1))) {    //Pawn in beginning of safety zone
                    possibleMoves.push_back(1 + 11*(color-1));
                }
                else if (((selectedPawn >= (40 + 11*(color-1))) && (selectedPawn <= (43 + 11*(color-1)))) || (selectedPawn == (0 + 11*(color-1))) || (selectedPawn == (1 + 11*(color-1)))) {    //Pawn too close to home to move ten
                    if (selectedPawn == 0) {
                        possibleMoves.push_back(43);
                    } else {
                        possibleMoves.push_back(selectedPawn - 1);
                    }
                }
                else if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 10 spaces forwards or 1 space backwards
                    possibleMoves.push_back((selectedPawn + 10)%44);
                    possibleMoves.push_back(selectedPawn - 1);
                }
                break;
            case 11:
                if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                for (int i = 0; i < 44; i++) {
                    if(Board->currentLocations[i] != color && Board->currentLocations[i] != 0) {
                        possibleMoves.push_back(i);
                    }
                }
                if (selectedPawn == ((37 + (11*(color-1))))%44) {  //If pawn is nine away from safety zone
                    possibleMoves.push_back(47 + (9*(color-1)));
                }
                else if (selectedPawn == ((36 + (11*(color-1))))%44) {  //If pawn is ten away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                }
                else if (selectedPawn == ((35 + (11*(color-1)))%44)) {  //If pawn is eleven away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if (selectedPawn == ((38 + 11*(color-1))%44)) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if (((selectedPawn >= (47 + 9*(color-1))) && (selectedPawn <= (49 + 9*(color-1)))) || (selectedPawn == (0 + 11*(color-1))) || (selectedPawn == (1 + 11*(color-1))) || ((selectedPawn >= ((39 + 11*(color-1))%44)) && (selectedPawn <= ((43 + 11*(color-1))%44)))) {    //Pawn too close to home
                    break;
                }
                else {  //Otherwise move 11 spaces forwards
                    possibleMoves.push_back((selectedPawn + 11)%44);
                }
                break;
            case 12:
                if (selectedPawn == ((36 + (11*(color-1))))%44) {  //If pawn is ten away from safety zone
                    possibleMoves.push_back(47 + (9*(color-1)));
                }
                else if (selectedPawn == ((35 + (11*(color-1))))%44) {  //If pawn is eleven away from safety zone
                    possibleMoves.push_back(48 + (9*(color-1)));
                }
                else if (selectedPawn == ((34 + (11*(color-1)))%44)) {  //If pawn is twelve away from safety zone
                    possibleMoves.push_back(49 + (9*(color-1)));
                }
                else if (selectedPawn == ((37 + 11*(color-1))%44)) {    //Pawn can move to home
                    possibleMoves.push_back(Board->findNextOpenHome(color));
                }
                else if (((selectedPawn >= (47 + 9*(color-1))) && (selectedPawn <= (49 + 9*(color-1)))) || (selectedPawn == (0 + 11*(color-1))) || (selectedPawn == (1 + 11*(color-1))) || ((selectedPawn >= ((38 + 11*(color-1))%44)) && (selectedPawn <= ((43 + 11*(color-1))%44)))) {    //Pawn too close to home
                    break;
                }
                else if ((selectedPawn >= (50 + 9*(color-1))) && (selectedPawn <= (52 + 9*(color-1)))) {    //Pawn can't move in start
                    break;
                }
                else if ((selectedPawn >= (44 + 9*(color-1))) && (selectedPawn <= (46 + 9*(color-1)))) {    //Pawn can't move in home
                    break;
                }
                else {  //Otherwise move 12 spaces forwards
                    possibleMoves.push_back((selectedPawn + 12)%44);
                }
                break;
        }

        //Handle if a possible move lands on one of your own pawns
        for (int i = 0; i < possibleMoves.size(); i++) {
            Player->handleSelfCollision(Board, possibleMoves[i], possibleMoves, i);
        }
        return possibleMoves;
    }

    void LogicCalculations::checkSecondPawnDistance(LogicBoard* Board, LogicPlayer* Player, int distances, vector<int> &possibleMoves) { 
        vector<int> otherPawnLocations = Board->findOtherPawnLocations(Player);
        int color = Player->getPlayerColor(Player->currentPlayer);
        vector<int> strikes(distances);
        //Determine if the other pawns can move the leftover distance
        for (int i = 0; i < otherPawnLocations.size(); i++) {
            for (int j = 1; j <= distances; j++) {
                switch (j) {
                    case 1:
                        //Check if the other pawn can move 6 spaces
                        if ((otherPawnLocations[i] >= (44 + 9*(color-1))) && (otherPawnLocations[i] <= (52 + 9*(color-1)))) {    //If pawn is in home, safety zone, or start
                            strikes[j-1]++;
                        }
                        else if ((otherPawnLocations[i] == (0 + 11*(color-1))) || (otherPawnLocations[i] == (1 + 11*(color-1)))) { //If pawn is too close to home
                            strikes[j-1]++;
                        }
                        else {
                            strikes[j-1] = 0;
                        }
                    break;
                    case 2:
                        //Check if the other pawn can move 5 spaces
                        if ((otherPawnLocations[i] >= (44 + 9*(color-1))) && (otherPawnLocations[i] <= (52 + 9*(color-1)))) {    //If pawn is in home, safety zone, or start
                            strikes[j-1]++;
                        }
                        else if (otherPawnLocations[i] == (0 + 11*(color-1))) { //If pawn is too close to home
                            strikes[j-1]++;
                        }
                        else {
                            strikes[j-1] = 0;
                        }
                    break;
                    case 3:
                        //Check if the other pawn can move 4 spaces
                        if ((otherPawnLocations[i] >= (44 + 9*(color-1))) && (otherPawnLocations[i] <= (52 + 9*(color-1)))) {    //If pawn is in home, safety zone, or start
                            strikes[j-1]++;
                        }
                        else {
                            strikes[j-1] = 0;
                        }
                    break;
                    case 4:
                        //Check if the other pawn can move 3 spaces
                        if ((otherPawnLocations[i] >= (44 + 9*(color-1))) && (otherPawnLocations[i] <= (46 + 9*(color-1)))) {    //If pawn is in home
                            strikes[j-1]++;
                        }
                        else if ((otherPawnLocations[i] >= (50 + 9*(color-1))) && (otherPawnLocations[i] <= (52 + 9*(color-1)))) {  //If pawn is in start
                            strikes[j-1]++;
                        }
                        else if ((otherPawnLocations[i] == (47 + 9*(color-1))) || (otherPawnLocations[i] == (48 + 9*(color-1)))) {  //If pawn in safety zone is too close to home
                            strikes[j-1]++;
                        }
                        else {
                            strikes[j-1] = 0;
                        }
                    break;
                    case 5:
                        //Check if the other pawn can move 2 spaces
                        if ((otherPawnLocations[i] >= (44 + 9*(color-1))) && (otherPawnLocations[i] <= (46 + 9*(color-1)))) {    //If pawn is in home
                            strikes[j-1]++;
                        }
                        else if ((otherPawnLocations[i] >= (50 + 9*(color-1))) && (otherPawnLocations[i] <= (52 + 9*(color-1)))) {  //If pawn is in start
                            strikes[j-1]++;
                        }
                        else if (otherPawnLocations[i] == (47 + 9*(color-1))) {  //If pawn in safety zone is too close to home
                            strikes[j-1]++;
                        }
                        else {
                            strikes[j-1] = 0;
                        }
                    break;
                    case 6:
                        ///Check if the other pawn can move 1 space
                        if ((otherPawnLocations[i] >= (44 + 9*(color-1))) && (otherPawnLocations[i] <= (46 + 9*(color-1)))) {    //If pawn is in home
                            strikes[j-1]++;
                        }
                        else if ((otherPawnLocations[i] >= (50 + 9*(color-1))) && (otherPawnLocations[i] <= (52 + 9*(color-1)))) {  //If pawn is in start
                            strikes[j-1]++;
                        }
                        else {
                            strikes[j-1] = 0;
                        }
                    break;
                }
            }
            //Delete moves that can't be made
            for (int k = distances; k >= 0; k--) {
                if (strikes[k] == 2) {
                    possibleMoves.erase(possibleMoves.begin() + k);
                }
            }
        }
    }

    //This function can be used to find the difference between two locations up to 7 spaces
    int LogicCalculations::getDistance(LogicPlayer* Player, int start, int end) {
        int color = Player->getPlayerColor(Player->currentPlayer);
        if ((start >= 0) && (start <= 43) && (end >= 0) && (end <= 43)) { //Normal locations
            if (start <= end) {
                return end - start;
            } else {
                return 44 - start + end;
            }
        } 
        else if ((end >= (44 + 9*(color-1))) && (end <= (46 + 9*(color-1)))) { //Final location in Home
            if ((start == (47 + 9*(color-1))) || (start == (49 + 9*(color-1)))) { //Start is in Safety Zone
                return (start - (46 + 9*(color-1)));
            }
            else if (start == (1 + 11*(color-1))) { //Start is one away from Safety Zone
                return 4;
            }
            else if (start == (0 + 11*(color-1))) { //Start is two away from Safety Zone
                return 5;
            }
            else if (start == ((43 + 11*(color-1))%44)) { //Start is three away from Safety Zone
                return 6;
            }
            else if (start == ((42 + 11*(color-1))%44)) { //Start is four away from Safety Zone
                return 7;
            }
        }
        else if (end == (47 + 9*(color-1))) { //Final location in Safety Zone near Home
            if ((start == (48 + 9*(color-1))) || (start == (49 + 9*(color-1)))) { //Start is in Safety Zone
                return (start - (47 + 9*(color-1)));
            }
            else if (start == (1 + 11*(color-1))) { //Start is one away from Safety Zone
                return 3;
            }
            else if (start == (0 + 11*(color-1))) { //Start is two away from Safety Zone
                return 4;
            }
            else if (start == ((43 + 11*(color-1))%44)) { //Start is three away from Safety Zone
                return 5;
            }
            else if (start == ((42 + 11*(color-1))%44)) { //Start is four away from Safety Zone
                return 6;
            }
            else if (start == ((41 + 11*(color-1))%44)) { //Start is five away from Safety Zone
                return 7;
            }
        }
        else if (end == (48 + 9*(color-1))) { //Final location in Safety Zone middle
            if (start == (49 + 9*(color-1))) { //Start is in Safety Zone
                return 1;
            }
            else if (start == (1 + 11*(color-1))) { //Start is one away from Safety Zone
                return 2;
            }
            else if (start == (0 + 11*(color-1))) { //Start is two away from Safety Zone
                return 3;
            }
            else if (start == ((43 + 11*(color-1))%44)) { //Start is three away from Safety Zone
                return 4;
            }
            else if (start == ((42 + 11*(color-1))%44)) { //Start is four away from Safety Zone
                return 5;
            }
            else if (start == ((41 + 11*(color-1))%44)) { //Start is five away from Safety Zone
                return 6;
            }
            else if (start == ((40 + 11*(color-1))%44)) { //Start is six away from Safety Zone
                return 7;
            }
        }
        else if (end == (49 + 9*(color-1))) { //Final location in Safety Zone near Start
            if (start == (1 + 11*(color-1))) { //Start is one away from Safety Zone
                return 1;
            }
            else if (start == (0 + 11*(color-1))) { //Start is two away from Safety Zone
                return 2;
            }
            else if (start == ((43 + 11*(color-1))%44)) { //Start is three away from Safety Zone
                return 3;
            }
            else if (start == ((42 + 11*(color-1))%44)) { //Start is four away from Safety Zone
                return 4;
            }
            else if (start == ((41 + 11*(color-1))%44)) { //Start is five away from Safety Zone
                return 5;
            }
            else if (start == ((40 + 11*(color-1))%44)) { //Start is six away from Safety Zone
                return 6;
            }
            else if (start == ((39 + 11*(color-1))%44)) { //Start is seven away from Safety Zone
                return 7;
            }
        }
        return -1;
    }
}