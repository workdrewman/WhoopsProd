// =======================================================
// Description: This header file contains the declaration
//              of the RFID helper functions.
// =======================================================

///////////////////////////
// PIN MAPPING
// 3.3    ->    3.3
// Reset  ->    D15
// GND    ->    GND
// MISO   ->    D19
// MOSI   ->    D23
// SCK    ->    D18
// SDA    ->    D5
///////////////////////////

#ifndef RFID_SCANNER_H
#define RFID_SCANNER_H

#include <MFRC522.h>
#include <SPI.h>

namespace rfid{

const int kSSPin{5};
const int kRSTPin{15};

struct Card {
    byte uid[7];       // UID of the card
    uint8_t name;      // Name of the card
};

const Card kAllowedCards[] = {
  {{0x04, 0xCD, 0x89, 0xA6, 0x67, 0x26, 0x81}, 1},
  {{0x04, 0x67, 0x4D, 0xA4, 0x67, 0x26, 0x81}, 2},
  {{0x04, 0x1D, 0xCF, 0xA5, 0x67, 0x26, 0x81}, 3},
  {{0x04, 0x8B, 0xF4, 0xA7, 0x67, 0x26, 0x81}, 4},
  {{0x04, 0x2A, 0x8C, 0xA2, 0x67, 0x26, 0x81}, 5},
  {{0x04, 0x75, 0xDF, 0xA5, 0x67, 0x26, 0x81}, 7},
  {{0x04, 0xC4, 0x63, 0xA5, 0x67, 0x26, 0x81}, 8},
  {{0x04, 0x31, 0x98, 0xA9, 0x67, 0x26, 0x81}, 10},
  {{0x04, 0x8C, 0xDD, 0xA5, 0x67, 0x26, 0x81}, 11},
  {{0x04, 0x6D, 0x78, 0xA2, 0x67, 0x26, 0x81}, 12},
  {{0x04, 0x7F, 0x36, 0xA4, 0x67, 0x26, 0x81}, 0},
};
const int kNumAllowedCards = sizeof(kAllowedCards) / sizeof(kAllowedCards[0]);
const uint8_t kInvalidCardName{99};

class RfidScanner{

 public:
  /// @brief Constructor to initialize the scanner
  RfidScanner();

  /// @brief Function that busy waits until card is scanned
  /// @return The number the the rfid card corresponds to
  uint8_t scanCard();

  /// @brief Function to find if the scanned UID matches an allowed card
  /// @param scannedUID rfid card UID
  /// @param uidSize size of the UID
  /// @return returns the name (number on card)
  uint8_t getCardName(byte *scannedUID, byte uidSize);

 private:
  /// @brief Helper routine to dump a byte array as hex values to Serial. 
  /// @param buffer the byte array from rfid
  /// @param bufferSize size of the buffer
  void printHex(byte *buffer, byte bufferSize);

  /// @brief Helper routine to dump a byte array as dec values to Serial.
  /// @param buffer the byte array from rfid
  /// @param bufferSize size of the buffer
  void printDec(byte *buffer, byte bufferSize);

  MFRC522 _rfid_scanner;
};

} // namespace rfid
#endif // RFID_SCANNER_H