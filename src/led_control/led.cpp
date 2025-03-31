// =======================================================
// Description: This header file contains the implementation
//              of helper functions to illuminate tiles
//              when indicating a move.
// =======================================================

#include "led_control/led.hpp"

#include "FastLED.h"
#include <vector> // for vector

#include "game_logic/logic_board.hpp" // for LogicBoard

int start_pos;
std::vector<int> possible_moves_led;
CRGB led_color;

namespace led_control
{

const int kBoardSideLength{11};

void indicate_move(int from, int to, CRGB color)
{
  FastLED.leds()[to] = color;
  for (int i = from; i < to; ++i) {
    if (i - 2 >= from) {
      FastLED.leds()[i-2].fadeLightBy(230);
      FastLED.leds()[i-1].fadeLightBy(200);
    }
    FastLED.leds()[i] = color;
    FastLED.leds()[i].fadeLightBy(200);
    FastLED.show();
    FastLED.delay(75);
  }
  FastLED.leds()[to-2].fadeLightBy(230);
  FastLED.leds()[to-1].fadeLightBy(200);
  FastLED.delay(75);
  FastLED.show();
  FastLED.clear();
  FastLED.show();
}

void indicate_move(std::vector<int> indexes, CRGB color)
{
  FastLED.leds()[indexes[0]] = color;
  for (int i = 0; i < indexes.size(); ++i) {
    if (i >= 2) {
      FastLED.leds()[indexes[i-2]].fadeLightBy(230);
      FastLED.leds()[indexes[i-1]].fadeLightBy(200);
    }
    FastLED.leds()[indexes[i]] = color;
    FastLED.leds()[indexes[i]].fadeLightBy(200);
    FastLED.show();
    FastLED.delay(75);
  }
  FastLED.leds()[indexes[indexes.size()-2]].fadeLightBy(230);
  FastLED.leds()[indexes[indexes.size()-1]].fadeLightBy(200);
  FastLED.delay(75);
  FastLED.show();
  FastLED.clear();
  FastLED.show();
}

void demo_move(int card_scanned) {
  FastLED.leds()[0] = CRGB::Yellow;
  FastLED.leds()[11] = CRGB::Green;
  FastLED.leds()[22] = CRGB::Red;
  FastLED.leds()[33] = CRGB::Blue;
  for (int i = 0; i < card_scanned; ++i) {
    if (i - 2 >= 0) {
      FastLED.leds()[i-2].fadeLightBy(230);
      FastLED.leds()[i-1].fadeLightBy(200);
      FastLED.leds()[i+11-2].fadeLightBy(230);
      FastLED.leds()[i+11-1].fadeLightBy(200);
      FastLED.leds()[i+22-2].fadeLightBy(230);
      FastLED.leds()[i+22-1].fadeLightBy(200);
      FastLED.leds()[i+33-2].fadeLightBy(230);
      FastLED.leds()[i+33-1].fadeLightBy(200);
    }
    FastLED.leds()[i] = CRGB::Yellow;
    FastLED.leds()[i].fadeLightBy(200);
    FastLED.leds()[i+11] = CRGB::Green;
    FastLED.leds()[i+11].fadeLightBy(200);
    FastLED.leds()[i+22] = CRGB::Red;
    FastLED.leds()[i+22].fadeLightBy(200);
    FastLED.leds()[i+33] = CRGB::Blue;
    FastLED.leds()[i+33].fadeLightBy(200);
    FastLED.show();
    FastLED.delay(75);
  }
  FastLED.leds()[card_scanned-2].fadeLightBy(230);
  FastLED.leds()[card_scanned-1].fadeLightBy(200);
  FastLED.leds()[card_scanned-2+11].fadeLightBy(230);
  FastLED.leds()[card_scanned-1+11].fadeLightBy(200);
  FastLED.leds()[card_scanned-2+22].fadeLightBy(230);
  FastLED.leds()[card_scanned-1+22].fadeLightBy(200);
  FastLED.leds()[card_scanned-2+33].fadeLightBy(230);
  FastLED.leds()[card_scanned-1+33].fadeLightBy(200);
  FastLED.delay(75);
  FastLED.show();
  FastLED.clear();
  FastLED.show();
}

//Yellow: 1, Green: 2, Red: 3, Blue: 4
CRGB number_to_color(int color)
{
  switch (color){
    case 1:
      return CRGB::Yellow;
    case 2:
      return CRGB::Red;
    case 3:
      return CRGB::Green;
    case 4:
      return CRGB::Blue;
  }
}

void showCorrectPositions(logic::LogicBoard* board) {
  FastLED.clear();
  FastLED.show();
  for (int i = 0; i < logic::kBoardSize; i++) {
      if (board->currentLocations[i] == 0) {
          continue;
      } else {
          FastLED.leds()[i] = led_control::number_to_color(board->currentLocations[i]);
      }
  }
  FastLED.show();
}

void ledTask(void *pvParameters) {        
  while (1) {
      // Serial.println("LED sequence running...");
      FastLED.leds()[start_pos] = led_color;
      for (int move : possible_moves_led) {
          FastLED.leds()[move] = led_color;
      }
      FastLED.show();
      vTaskDelay(pdMS_TO_TICKS(500));

      for (int move : possible_moves_led) {
          FastLED.leds()[move] = CRGB::Black;
      }
      FastLED.show();
      vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void indicate_moves(const vector<int>& possibleMoves, int color, int start_tile, TaskHandle_t* taskHandle)
{
    possible_moves_led = possibleMoves;
    led_color = led_control::number_to_color(color);
    start_pos = start_tile;

    FastLED.clear();
    FastLED.show();
    
    xTaskCreate(led_control::ledTask, "LED Task", 4096, NULL, 1, taskHandle);
}

void showWinner(int player_number)
{
  CRGB color = number_to_color(player_number);
  for (int itr=0; itr < 10; itr++) {
    for (int tile=0; tile < kBoardSideLength+1; tile++){
      for (int side=0; side < 2; side++) {
        FastLED.leds()[side*kBoardSideLength*2 + tile] = color;
        FastLED.leds()[side*kBoardSideLength + tile + 11] = CRGB::Black;
        FastLED.show();
        vTaskDelay(pdMS_TO_TICKS(100));
      }
    }
    for (int tile=0; tile < kBoardSideLength+1; tile++){
      for (int side=0; side < 2; side++) {
        FastLED.leds()[side*kBoardSideLength*2 + tile] = CRGB::Black;
        FastLED.leds()[side*kBoardSideLength + tile + 11] = color;
        FastLED.show();
        vTaskDelay(pdMS_TO_TICKS(100));
      }
    }
  }
}

} // namespace led_control
