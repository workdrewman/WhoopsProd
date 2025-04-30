// =======================================================
// Description: This header file contains the implementation
//              of helper functions to illuminate tiles
//              when indicating a move.
// =======================================================

#include "led_control/led.hpp"

#include "FastLED.h"
#include <vector> // for vector

#include "game_logic/logic_board.hpp" // for LogicBoard


namespace led_control
{
int g_start_pos;
std::vector<int> g_possible_moves_led;
CRGB g_led_color;
int g_num_players = 0;
led_control::SlideStruct g_slide;

TaskHandle_t led_task_handle = NULL;
TaskHandle_t start_pos_handle = NULL;
TaskHandle_t slide_task_handle = NULL;

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
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  FastLED.leds()[indexes[indexes.size()-2]].fadeLightBy(230);
  FastLED.leds()[indexes[indexes.size()-1]].fadeLightBy(200);
  vTaskDelay(pdMS_TO_TICKS(100));
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
    default:
      return CRGB::Black;
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
    if (g_start_pos < 80) {
      FastLED.leds()[g_start_pos] = g_led_color;
    }
    for (int move : g_possible_moves_led) {
      FastLED.leds()[move] = g_led_color;
    }
    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(500));

    for (int move : g_possible_moves_led) {
      FastLED.leds()[move] = CRGB::Black;
    }
    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void stopLedTask() {
  if (eTaskGetState(led_task_handle) != eDeleted) {
    vTaskSuspend(led_task_handle);
  }
}

void indicate_moves(const vector<int>& possibleMoves, int color, int start_tile)
{
  g_possible_moves_led = possibleMoves;
  g_led_color = led_control::number_to_color(color);
  g_start_pos = start_tile;

  FastLED.clear();
  FastLED.show();
  
  if (led_task_handle == NULL) {
    xTaskCreate(
      led_control::ledTask,
      "LED Task",
      2048,
      NULL,
      1,
      &led_task_handle
    );
  }
  else {
    vTaskResume(led_task_handle);
  }
}

void stopIndicateMoves() {
  if (eTaskGetState(led_task_handle) != eDeleted) {
    vTaskSuspend(led_task_handle);
  }
}

void showPlayerPositions(int player_number, const logic::LogicBoard& board) {
  g_possible_moves_led.clear();
  for (int i = 0; i < logic::kBoardSize; i++) {
    if (board.currentLocations[i] == player_number) {
      g_possible_moves_led.push_back(i);
      Serial.println("Player " + String(player_number) + " is at tile " + String(i));
    }
  }
  g_led_color = led_control::number_to_color(player_number);
  g_start_pos = 255;

  if (led_task_handle == NULL) {
    xTaskCreate(
      led_control::ledTask,
      "Flash Current Player Positions",
      2048,
      NULL,
      1,
      &led_task_handle
    );
  }
  else {
    vTaskResume(led_task_handle);
  }
}

void stopPlayerPositions() {
  if (eTaskGetState(led_task_handle) != eDeleted) {
    vTaskSuspend(led_task_handle);
  }
}

void showStartPositions(int num_players) {
  g_num_players = num_players;

  if (start_pos_handle == NULL) {
    xTaskCreate(
      led_control::prvShowStartPositions,
      "Show Start Positions",
      2048,
      NULL,
      1,
      &start_pos_handle
    );
  } else {
    vTaskResume(start_pos_handle);
  }
}

void stopStartPositions() {
  if (eTaskGetState(start_pos_handle) != eDeleted) {
    vTaskSuspend(start_pos_handle);
  }
}

void prvShowStartPositions(void *pvParameters) {
  Serial.println("Showing start positions for " + String(g_num_players) + " players");
  while(1) {
    for (int i = 0; i < g_num_players; i++) {
      for (int j = 0; j < 3; j++) {
        FastLED.leds()[logic::kStartLocations[i*3+j]] = number_to_color(i+1);
      }
    }
    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(500));
    for (int i = 0; i < g_num_players; i++) {
      for (int j = 0; j < 3; j++) {
        FastLED.leds()[logic::kStartLocations[i*3+j]] = CRGB::Black;
      }
    }
    FastLED.show();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void slidePiece(SlideStruct slide) {
  g_slide = slide;

  if (slide_task_handle == NULL) {
    xTaskCreate(
      led_control::prvShowSlide,
      "Show Start Positions",
      2048,
      NULL,
      1,
      &slide_task_handle
    );
  }
  else {
    vTaskResume(slide_task_handle);
  }
}

void stopSlide() {
  if (eTaskGetState(slide_task_handle) != eDeleted) {
    vTaskSuspend(slide_task_handle);
    slide_task_handle = NULL;
  }
}

void prvShowSlide(void *pvParameters) {
  std::vector<int> indexes;
  for (int i = g_slide.start_location; i <= g_slide.end_location; ++i) {
    indexes.push_back(i);
  }
  CRGB color = number_to_color(g_slide.color);
  
  while(1) {
    FastLED.leds()[indexes[0]] = color;
    for (int i = 0; i < indexes.size(); ++i) {
      if (i >= 2) {
        FastLED.leds()[indexes[i-2]].fadeLightBy(230);
        FastLED.leds()[indexes[i-1]].fadeLightBy(200);
      }
      FastLED.leds()[indexes[i]] = color;
      FastLED.leds()[indexes[i]].fadeLightBy(200);
      FastLED.show();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    FastLED.leds()[indexes[indexes.size()-2]].fadeLightBy(230);
    FastLED.leds()[indexes[indexes.size()-1]].fadeLightBy(200);
    vTaskDelay(pdMS_TO_TICKS(100));
    FastLED.show();
    FastLED.clear();

    FastLED.show();
  }
}

void showWinner(int player_number)
{
  FastLED.clear();
  FastLED.show();
  CRGB color = number_to_color(player_number);
  for (int itr=0; itr < 10; itr++) {
    for (int tile=0; tile < kBoardSideLength*2; tile++){
      for (int side=0; side < 2; side++) {
        FastLED.leds()[side*kBoardSideLength*2 + tile] = color;
        FastLED.leds()[(side*kBoardSideLength*2 + tile + 10) % 44] = CRGB::Black;
        FastLED.show();
        vTaskDelay(pdMS_TO_TICKS(10));
      }
    }
  }
  for (int i=0; i < 44; i++) {
    FastLED.leds()[i] = color;
  }
  FastLED.show();
}

} // namespace led_control
