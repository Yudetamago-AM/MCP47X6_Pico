#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../Src/MCP47X6.h"

// i2c1
const uint8_t sda = 6;
const uint8_t scl = 7;

// Instantiate objects used in this project
MCP47X6 theDAC=MCP47X6(i2c1, MCP47X6_DEFAULT_ADDRESS);

int main(void) {
  // setup
  theDAC.begin(sda, scl, MCP47X6_VREF_VREFPIN & MCP47X6_GAIN_2X);
  theDAC.setVReference(MCP47X6_VREF_VDD);
  theDAC.setGain(MCP47X6_GAIN_1X);
  theDAC.saveSettings();

  // loop
  while (1) {
    uint16_t level = 0x123;
    uint8_t byteLevel = 0x45;
    theDAC.setOutputLevel(level);
    theDAC.setOutputLevel(byteLevel);
    theDAC.powerDown();
  }

  return 0;
}