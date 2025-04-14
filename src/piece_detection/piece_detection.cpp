// =======================================================
// Description: This header file contains the implementation
//              of helper functions to bring I2C piece detection
//              to the Whoops! game
// =======================================================

#include "piece_detection/piece_detection.hpp"
#include "game_logic/logic_board.hpp"

#include <Wire.h> // for I2C communication
#include <algorithm> // for std::find

namespace piece_detection
{

PieceDetection::PieceDetection(size_t chips) : _num_chips{chips}, _sensor_data(chips*2, 0x0) {}

void PieceDetection::readMCPInputs() 
{
  uint8_t data;
  for (size_t i = 0; i < _num_chips; i++) {
    data = readMCPInput(kMCP23017_StartAddr + i, 0x12); // Port A
    writeData(i, 0, data);
    data = readMCPInput(kMCP23017_StartAddr + i, 0x13); // Port B
    writeData(i, 1, data);
  }
}


void PieceDetection::initMCP23017()
{
  Wire.begin();
  for (size_t i = 0; i < _num_chips; i++) {
    initMCP23017(kMCP23017_StartAddr + i);
  }
}

uint8_t PieceDetection::readMCPInput(uint8_t addr, uint8_t gpioReg)
{
  Wire.beginTransmission(addr);
  Wire.write(gpioReg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);

  if (Wire.available()) {
      return Wire.read(); // Read the byte from MCP23017
  }
  return 0xFF; // Default value if nothing is read
}


uint8_t PieceDetection::getData(uint8_t chip, uint8_t index)
{
  std::lock_guard<std::mutex> lock(_data_mutex);  // Lock mutex
  return _sensor_data[chip*2 + index];  // Safe access
}

uint8_t PieceDetection::getDataBlock(uint8_t chip, uint8_t port)
{
  return getData(chip, port);
}

uint8_t PieceDetection::getDataSingle(uint8_t index)
{
  uint8_t bitIndex = index % 8;
  uint8_t dataIndex = index / 8;
  std::lock_guard<std::mutex> lock(_data_mutex);  // Lock mutex
  return (_sensor_data[dataIndex] >> bitIndex) & 1;
}

void PieceDetection::initMCP23017(uint8_t addr)
{
  // Configure PORT A and PORT B as inputs
  Wire.beginTransmission(addr);
  Wire.write(0x00);  // IODIRA register (Port A)
  Wire.write(0xFF);  // Set all as inputs
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  Wire.write(0x01);  // IODIRB register (Port B)
  Wire.write(0xFF);  // Set all as inputs
  Wire.endTransmission();

  // Enable pull-up resistors on all inputs
  Wire.beginTransmission(addr);
  Wire.write(0x0C);  // GPPUA register (Port A)
  Wire.write(0xFF);  // Enable pull-ups
  Wire.endTransmission();

  Wire.beginTransmission(addr);
  Wire.write(0x0D);  // GPPUB register (Port B)
  Wire.write(0xFF);  // Enable pull-ups
  Wire.endTransmission();
}

void PieceDetection::writeData(uint8_t chip, uint8_t port, uint8_t value)
{
  uint8_t index = (chip * 2) + port;
  if (index < 0 || index >= _sensor_data.size()) return;  // Bounds check

  std::lock_guard<std::mutex> lock(_data_mutex);  // Lock mutex
  uint8_t old_value = _sensor_data[index];  // Read old value
  if (old_value != value) {
    std::vector<uint8_t> to_remove;
    for (int i = 0; i < 8; i++) {
      if ((old_value & (1 << i)) != (value & (1 << i))) {
        if (std::find(_changed_sensors.begin(), _changed_sensors.end(), index * 8 + i) == _changed_sensors.end()) {
          // Only add if not already present
          _changed_sensors.push_back(index * 8 + i);  // Store changed sensor index
        } else {
          // If already present, mark for removal
          to_remove.push_back(index * 8 + i);
        }
      }
    }
    for (auto idx : to_remove) {
      _changed_sensors.erase(std::remove(_changed_sensors.begin(), _changed_sensors.end(), idx), _changed_sensors.end());
    }
  }
  _sensor_data[index] = value;  // Remove volatile inside lock
}

std::vector<uint8_t> PieceDetection::getDataCopy()
{
  std::lock_guard<std::mutex> lock(_data_mutex);  // Lock mutex
  return _sensor_data;  // Return a copy of the data vector
}

bool PieceDetection::hasChangedSensor()
{
  if (_changed_sensors.size() == 0) {
    return false;  // No changed sensors
  }
  return true;
}

std::vector<uint8_t> PieceDetection::getChangedSensors()
{
  std::vector<uint8_t> copy_changed = _changed_sensors;
  _changed_sensors = {};
  return copy_changed;
}

void PieceDetection::updateBoard(logic::LogicBoard* Board)
{
  std::vector<uint8_t> data_copy = getDataCopy();
  int index = 0;
  for (size_t i = 0; i < data_copy.size(); i++) {
    for (size_t j = 0; j < 8; j++) {
      index = i * 8 + j;
      if (kSensorMap.find(index) == kSensorMap.end()){
        continue;
      }
      Board->currentLocations[kSensorMap.at(index)] = ((data_copy[i] >> j) & 1) ^ 1;
    }
  }
}


} // namespace piece_detection
