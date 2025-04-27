#include <Arduino.h>
extern HardwareSerial mySerial; // Declare mySerial as an external variable

namespace audio {

void playTrack(byte filenum) {
  mySerial.write(0x7E);
  mySerial.write(0xFF);
  mySerial.write(0x06);
  mySerial.write(0x03);
  mySerial.write(0x00);
  mySerial.write(0x00);
  mySerial.write(filenum);
  mySerial.write(0xEF);
}

void setVolume(byte volume) {
  if (volume > 30) volume = 30; // Ensure volume is within range
  mySerial.write(0x7E); // Start byte
  mySerial.write(0xFF); // Version
  mySerial.write(0x06); // Length
  mySerial.write(0x06); // Command: Set volume
  mySerial.write(0x00); // Feedback (0 = no feedback)
  mySerial.write(0x00); // High byte of parameter (always 0 for volume)
  mySerial.write(volume); // Low byte of parameter (volume level)
  mySerial.write(0xEF); // End byte
}

}