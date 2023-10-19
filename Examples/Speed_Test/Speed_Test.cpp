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
  stdio_init_all();
  sleep_ms(1500);
  printf("Speed_Test\n");

  // setup
  bool status = theDAC.begin(sda, scl, MCP47X6_VREF_VREFPIN & MCP47X6_GAIN_2X);
  printf("%s\n", (status ? "MCP47X6 connection successful" : "MCP47X6 connection failed"));
  theDAC.setVReference(MCP47X6_VREF_VDD);
  theDAC.setGain(MCP47X6_GAIN_1X);
  theDAC.saveSettings();

  // loop
  while (1) {
    theDAC.setOutputLevel((uint16_t) 0x000);
    theDAC.setOutputLevel((uint16_t) 0xFFF);
  }

  return 0;
}