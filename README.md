# Whoops! Release 1.0
To be used along with the Whoops! board, pawns, and RFID tags.

## Purpose
Our project seeks to recreate the classic game Sorry! in order to provide greater accessibility to players with cognitive and physical impairments (namely visual, audio, and motor function impairments). Our product, Whoops!, is targeted strictly towards the Toys with a Purpose program, directed by our sponsor Dr. Patrick Kitzman, and is designed to be used for either physical therapy or at-home play. 

## Features:
### Game Logic
Game logic interfaces with the various sensor systems in order to create a real-time Whoops! game interaction between 2-4 players.

### Piece Detection
Hall effect sensors detect magnets inside of game pieces to provide tracking information to the game logic.

### RFID Scanner
An RFID scanner is integrated into the game to scan the cards that represent the moves you can make.

### LED System
The light system highlights the pieces to show the correct spots as well as showing your possible moves when you pick up a piece. This is intended to aid the gameplay by getting getting rid of counting errors and reducing cheating.

## Implementation
### Game Logic
The game logic is based on the game Sorry!. The rules of the game are coded in so that only valid moves can be played. For example, a 1 or 2 can get you out of the Start tiles, or you can move forward 1 or 2 on a different pawn. An 11 is used to move forward 11 or swap places with an opponent. A Whoops! card will let you take a pawn from start and swap places with an opponent, sending the opponent back to their start.

### Piece Detection
Piece detection utilizes AH3326-P-B hall effect sensors under each game tile which store their state in MCP23017-E/SP IO expanders. To poll the state of all sensors, the microcontroller uses I2C commands to check each address. The status is then stored locally to the microcontroller and is updated every 50ms. If a sensor has changed, the changed sensor gets added to a list that the game logic interfaces with to check for changed sensors.

At the beginning of the game, all sensors are polled to check that pieces are in the start positions, but after that, the game only checks if the sensors have changed to reduce the complexity of the code. Because there are lights indicating where pieces should be, the game can assume that pieces will be on their correct spot. If they are not, the game logic knows which piece to expect movement from, so when a piece is moved from an incorrect spot it will not do anything.

Because the hall effect sensors are connected to IO expanders, the index that they correspond to is not the same as the tile number. Therefore, a map is used to interchange between the tile number and the sensor number in the IO expander. This is found in the header file.

### RFID Scanner
The RFID scanner uses an SPI communication bus. There are known RFID tags that correspond to a card value. When waiting for a card and a card is scanned, the scanner returns the value that the tag corresponds to, and the game logic can use that value to perform the turn. Because there were timeout issues with the scanner, the scanner initializes every so often to stay active.

### LED System
The LED system uses addressable RGB LEDs. To implement this, we use the FastLED library and add 80 LEDs. In order to access them, you can just include the FastLED.h header file and call FastLED.leds() and index into the array that this gives you access to. The tiles start on yellow's side and go around the perimeter, then they go through all the home and start positions. The tile number corresponds to the LED number as we have made the game. Various LED tasks are launched as needed to show where pieces should be, to show whose turn it is, to show a slide, and so on. All of these tasks are found in the led.cpp and led.h files.

## Demonstration
You can find a complete game playthrough here: https://youtu.be/xLeYQk_LBbo