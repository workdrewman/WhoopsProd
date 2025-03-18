// =======================================================
// Description: This header file contains the implementation
//              of helper functions to illuminate tiles
//              when indicating a move.
// =======================================================

#include "led_control/led.hpp"

#include "FastLED.h"
#include <vector> // for vector

namespace led_control
{

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

void indicate_possible_moves(int from, std::vector<int> to, int color)
{
  light_tiles(to, number_to_color(color));
}

void light_tiles(std::vector<int> tiles, CRGB color)
{
  for (size_t i = 0; i < tiles.size(); i++) {
    FastLED.leds()[tiles[0]] = color;
  }
  FastLED.show();
  FastLED.delay(500);
  FastLED.clear();
}

//Yellow: 1, Green: 2, Red: 3, Blue: 4
CRGB number_to_color(int color)
{
  switch (color){
    case 1:
      return CRGB::Yellow;
    case 2:
      return CRGB::Green;
    case 3:
      return CRGB::Red;
    case 4:
      return CRGB::Blue;
  }
}

} // namespace led_control
