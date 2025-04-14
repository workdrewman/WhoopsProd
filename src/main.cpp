#include <Arduino.h>
#include "game_logic/logic_controller.hpp" // for LogicController

#define NUM_LEDS 80
#define DATA_PIN 13


CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  set_max_power_in_volts_and_milliamps(5, 200);
  Serial.begin(9600);
  logic::LogicController* logicController = new logic::LogicController;
  logicController->startGame(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  // static uint8_t hue = 0; // Tracks the color hue
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   leds[i] = CHSV(hue, 255, 255); // Set each LED to the current hue
  // }
  // FastLED.show();
  // hue++; // Increment the hue for the next cycle
  // delay(20); // Small delay for smooth cycling
}
