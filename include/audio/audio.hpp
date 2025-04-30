#include <Arduino.h>
extern HardwareSerial mySerial; // Declare mySerial as an external variable

namespace audio {

void playTrack(byte filenum);

void setVolume(byte volume);

}

/*
AUDIO FILE KEY 
  audio::playTrack(1); // Play Move 1
  delay(8000);
  audio::playTrack(3); // Play Move 2 
  delay(7000);
  audio::playTrack(5); // Play Move 3 
  delay(5000);
  audio::playTrack(7); // Play Move 4
  delay(5000);
  audio::playTrack(9); // Play Move 5
  delay(5000);
  audio::playTrack(11); // Play Move 7
  delay(8000);
  audio::playTrack(13); // Play Move 8
  delay(4000);
  audio::playTrack(15); // Play Move 10
  delay(11000);
  audio::playTrack(17); // Play Move 11
  delay(8000);
  audio::playTrack(19); // Play Move 12
  delay(5000);
  audio::playTrack(21); // Play Whoops
  delay(12000);
  audio::playTrack(23); // Play Start Game
  delay(9000);
  audio::playTrack(25); // Play Yellow start
  delay(5000);
  audio::playTrack(27); // Play Pink scan
  delay(5000);
  audio::playTrack(29); // Play Blue scan
  delay(5000);
  audio::playTrack(31); // Play Green scan
  delay(5000);
  audio::playTrack(33); // Play Yellow scan
  delay(5000);
  audio::playTrack(35); // Play Collision
  delay(8000);
  audio::playTrack(37);  // Play slide
  delay(10000);
*/