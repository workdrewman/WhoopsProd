// =======================================================
// Description: This header file contains the defition
//              of helper functions to bring I2C piece detection
//              to the Whoops! game
// =======================================================

#ifndef PIECE_DETECTION_H
#define PIECE_DETECTION_H

#include <stdint.h> // for c types
#include <vector> // for vector of data
#include <mutex> // for mutex to make sensorData threadsafe
#include <map> // for index map

namespace logic {
  class LogicBoard;
}

namespace piece_detection
{
const auto kMCP23017_StartAddr{0x20};
const auto kMaxChips{8};
const std::map<uint8_t, uint8_t> kSensorMap = {
  {6, 0},
  {7, 1},
  {19, 2},
  {20, 3},
  {21, 4},
  {22, 5},
  {23, 6},
  {40, 7},
  {32, 8},
  {33, 9},
  {34, 10},
  {35, 11},
  {36, 12},
  {37, 13},
  {38, 14},
  {39, 15},
  {0, 38},
  {1, 39},
  {2, 40},
  {3, 41},
  {4, 42},
  {5, 43},
  {9, 44},
  {8, 45},
  {10, 46},
  {11, 47},
  {12, 48},
  {13, 49},
  {18, 50},
  {17, 51},
  {16, 52},
  {24, 53},
  {25, 54},
  {26, 55},
  {41, 56},
  {42, 57},
  {43, 58},
  {44, 59},
  {45, 60},
  {46, 61},
  {55, 16},
  {54, 17},
  {53, 18},
  {52, 19},
  {51, 20},
  {50, 21},
  {49, 22},
  {48, 23},
  {68, 24},
  {67, 25},
  {66, 26},
  {65, 27},
  {64, 28},
  {87, 29},
  {86, 30},
  {85, 31},
  {84, 32},
  {83, 33},
  {82, 34},
  {81, 35},
  {80, 36},
  {94, 37},
  {88, 79},
  {89, 78},
  {90, 77},
  {93, 76},
  {92, 75},
  {91, 74},
  {72, 73},
  {73, 72},
  {74, 71},
  {75, 70},
  {76, 69},
  {77, 68},
  {61, 67},
  {60, 66},
  {59, 65},
  {56, 64},
  {57, 63},
  {58, 62}
};

class PieceDetection{
 public:
  /// @brief Constructor
  /// @param chips The number of chips to be used
  PieceDetection(size_t chips);

  /// @brief Reads all the IO Expanders and updates the data
  void readMCPInputs();

  /// @brief Initialize all the IO Expanders
  void initMCP23017();

  /// @brief Get a single sensor data
  /// @param index The sensor's index
  /// @return 0 or 1 based on value of sensor
  uint8_t getDataSingle(uint8_t index);

  /// @brief Gets the 8 bit data block of a chip[port]
  /// @param chip The chip number
  /// @param port Port A, Port B (0/1)
  /// @return 8 bit data block
  uint8_t getDataBlock(uint8_t chip, uint8_t port);

  /// @brief returns a copy of the data vector
  /// @return copy of sensor data
  std::vector<uint8_t> getDataCopy();

  /// @brief returns a vector of all the changed sensors in order
  /// @return vector of sensor ids
  std::vector<uint8_t> getChangedSensors();

  /// @brief Check if any sensor has changed
  /// @return true if any sensor has changed
  bool hasChangedSensor();

  void updateBoard(logic::LogicBoard* Board);

  
 private:
  /// @brief Initialize a single MCP23017 chip
  /// @param addr The I2C address of the chip
  void initMCP23017(uint8_t addr);

  /// @brief Read the input from a specific MCP23017 chip
  /// @param addr The I2C address of the chip
  /// @param gpioReg The GPIO register to read from
  /// @return The read value from the register
  uint8_t readMCPInput(uint8_t addr, uint8_t gpioReg);

  /// @brief Write piece detection sensor values to private sensor data
  /// @param chip The chip number
  /// @param port The port number 0 (A) or 1 (B)
  /// @param value the 8 bit value to write
  void writeData(uint8_t chip, uint8_t port, uint8_t value);

  /// @brief Get the data from a specific chip and index
  /// @param chip The chip number
  /// @param index The port number 0 (A) or 1 (B)
  uint8_t getData(uint8_t chip, uint8_t index);

  std::vector<uint8_t> _sensor_data;
  std::mutex _data_mutex;
  uint8_t _num_chips;
  std::vector<uint8_t> _changed_sensors;
};
} // namespace piece_detection
#endif // PIECE_DETECTION_H
