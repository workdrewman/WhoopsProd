#include <Arduino.h>
#include "game_logic/logic_controller.hpp" // for LogicController
#include "led_control/led.hpp" // for led control

#define NUM_LEDS 80
#define DATA_PIN 13

CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  set_max_power_in_volts_and_milliamps(5, 100);
  Serial.begin(9600);
  logic::LogicController* logicController = new logic::LogicController;
  logicController->startGame(); 
}

void loop() {
  // logic::LogicController* logicController = new logic::LogicController;
  // logicController->startGame(); 
}
