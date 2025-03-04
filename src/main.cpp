#include <Arduino.h>
#include "game_logic/logic_controller.hpp" // for LogicController

// global logic controller
logic::LogicController* logicController = new logic::LogicController;

void setup() {
  Serial.begin(9600);
  logicController->startGame();
}

void loop() {
  // put your main code here, to run repeatedly:
}
