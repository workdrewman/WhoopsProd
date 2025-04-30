#include <Arduino.h>
#include "game_logic/logic_controller.hpp" // for LogicController
#include "led_control/led.hpp" // for led control
#include "audio/audio.hpp" // for audio control

#define NUM_LEDS 80
#define DATA_PIN 13
#define POT_PIN 27  // Pin for the potentiometer
HardwareSerial mySerial(1);

CRGB leds[NUM_LEDS];

long map_val(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void AudioPotTask(void *parameter) {
  int pot_read = 0; // Stores the most recent reading from the potentiometer
  uint32_t scaled_pot = 0; // Scaled value of the potentiometer
  uint32_t adj_pot = 0; // 0, 20-30
  while (true) {
    pot_read = analogReadRaw(POT_PIN); // Read the potentiometer value
    scaled_pot = pot_read * 30 / 4095; // Scale 0-4095 to 0-30
    // Map 0 to 0, and 1-30 to 20-30
    if (scaled_pot == 0) {
      adj_pot = 0;
    } else {
      adj_pot = map_val(scaled_pot, 1, 30, 20, 30); // Map 1-30 to 20-30
    }
    //audio::setVolume(scaled_pot); // Set brightness
    Serial.print(("Setting volume to: "));
    Serial.println(scaled_pot);
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Avoid overwhelming the CPU
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  set_max_power_in_volts_and_milliamps(5, 100);
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, 12, 14);
  
  // xTaskCreate(
  //   AudioPotTask,         // Task function
  //   "Potentiometer read",      // Name of the task
  //   2048,             // Stack size (in bytes)
  //   NULL,             // Task parameter
  //   2,                // Priority
  //   NULL              // Task handle
  // );

}

void loop() {
  FastLED.clear();
  FastLED.show();
  logic::LogicController* logicController = new logic::LogicController;
  audio::setVolume(28); // Set initial volume to 0
  // audio::playTrack(1);
  // logicController->startGame();

  Serial.println("Playing track 1");
  audio::playTrack(1); // Play Move 1
  delay(8000);
}
