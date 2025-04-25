// =======================================================
// Description: This header file contains the declaration
//              of helper functions to illuminate tiles
//              when indicating a move.
// =======================================================

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <CRGB.h> // for CRGB
#include <vector> // for vector
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> // for TaskHandle_t

namespace logic {
  class LogicBoard;
}

namespace led_control
{
  struct SlideStruct {
    int start_location;
    int end_location;
    int color;
  };

  /// @brief Method to indicate a move from one tile to another
  /// @param from Index of the current tile
  /// @param to Index of the tile to move to
  /// @param color Color to illuminate the path with
  void indicate_move(int from, int to, CRGB color);

  /// @brief Method to indicate a move from one tile to another
  /// @param indexes Indexes in order of where to move
  /// @param color Color to illuminate the path with
  void indicate_move(std::vector<int> indexes, CRGB color);

  /// @brief Method to indicate a move from one tile to another
  /// @param card_scanned number of the scanned card
  void demo_move(int card_scanned);
  
  CRGB number_to_color(int color);

  void showCorrectPositions(logic::LogicBoard* board);
  void ledTask(void *pvParameters);
  void indicate_moves(const std::vector<int>& possibleMoves, int color, int start_tile, TaskHandle_t* taskHandle);
  void showWinner(int player_number);
  void showStartPositions(int num_players, TaskHandle_t* taskHandle);
  void prvShowStartPositions(void *pvParameters);
  void slidePiece(SlideStruct slide, TaskHandle_t* taskHandle);

} //namespace led_control
#endif // LED_CONTROL_H