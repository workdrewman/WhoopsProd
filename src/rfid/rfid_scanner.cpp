// =======================================================
// Description: This header file contains the implementation
//              of the RFID helper functions.
// =======================================================

#include "rfid/rfid_scanner.hpp" // for RfidScanner class

namespace rfid {

RfidScanner::RfidScanner() : _rfid_scanner{kSSPin, kRSTPin} 
{
  SPI.begin(); // Init SPI bus
  _rfid_scanner.PCD_Init(); // Init MFRC522 
}

void RfidScanner::printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void RfidScanner::printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}

uint8_t RfidScanner::getCardName(byte *scannedUID, byte uidSize) {
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

uint8_t RfidScanner::scanCard()
{
  while(1){
    if (_rfid_scanner.PICC_IsNewCardPresent() && _rfid_scanner.PICC_ReadCardSerial()) {
      uint8_t new_card_id = getCardName(_rfid_scanner.uid.uidByte, _rfid_scanner.uid.size);
      if (new_card_id != kInvalidCardName) {
        Serial.println(("Tag accepted"));
        Serial.print(("Card Name: "));
        Serial.println(new_card_id);
        return new_card_id;
      } 
      
      else {
        Serial.println(("Tag not recognized"));
      }

      _rfid_scanner.PICC_HaltA(); // Stop reading after one scan
    }
  }
}

} // namespace rfid
