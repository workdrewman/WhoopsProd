// =======================================================
// Description: This header file contains the implementation
//              of the RFID helper functions.
// =======================================================

#include "rfid/rfid_scanner.hpp" // for helper function templates

namespace rfid {

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}

uint8_t getCardName(byte *scannedUID, byte uidSize) {
  for (int i = 0; i < kNumAllowedCards; i++) {
    // Check if UID sizes match
    if (uidSize != sizeof(kAllowedCards[i].uid)) {
      continue;
    }

    // Compare each byte of the UID
    bool match = true;
    for (byte j = 0; j < uidSize; j++) {
      if (scannedUID[j] != kAllowedCards[i].uid[j]) {
        match = false;
        break;
      }
    }

    // If a match is found, return the card name
    if (match) {
      return kAllowedCards[i].name;
    }
  }

  return kInvalidCardName; // No matches found
}

} // namespace rfid
