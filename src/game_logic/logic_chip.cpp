/// chip_logic.cpp
#include "game_logic/logic_chip.hpp"
#include "game_logic/logic_terminal.hpp"
#include <iostream>
#include <FastLED.h>

using namespace std;

namespace logic {
    LogicChip::LogicChip() : lastChip(-1) {}

    int LogicChip::waitForChip(LogicTerminal* Terminal) {
        //Add physical chip scanning later
        int chip = Terminal->t_GetChip();

        //check if chip is valid
        while ((chip < 0 || chip > 12) || chip == 6 || chip == 9) {
            //ERROR
            Serial.println("Invalid chip");
            chip = Terminal->t_GetChip();
        }
        return chip;
    }
}