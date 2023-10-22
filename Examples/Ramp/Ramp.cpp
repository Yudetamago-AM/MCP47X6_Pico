#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../Src/MCP47X6.h"

// i2c1
#define I2C_BUS i2c1
const uint8_t sda = 6;
const uint8_t scl = 7;

// Instantiate objects used in this project
MCP47X6 theDAC=MCP47X6(I2C_BUS, MCP47X6_DEFAULT_ADDRESS);

int main(void) {
  stdio_init_all();
  sleep_ms(1500);
  printf("Ramp\n");

  // setup
  i2c_init(I2C_BUS, 400 * 1000);
  gpio_set_function(sda, GPIO_FUNC_I2C);
  gpio_set_function(scl, GPIO_FUNC_I2C);
  gpio_pull_up(sda);
  gpio_pull_up(scl);
  //theDAC.begin(sda, scl, MCP47X6_VREF_VREFPIN_BUFFERED & MCP47X6_GAIN_2X);
  theDAC.setVReference(MCP47X6_VREF_VREFPIN_BUFFERED);
  theDAC.setGain(MCP47X6_GAIN_1X);
  bool status = theDAC.begin(sda, scl);
  printf("%s\n", (status ? "MCP47X6 connection successful" : "MCP47X6 connection failed"));


  // loop
  uint16_t level = 0;
  char c;
  while (1) {
    printf("level>>2: %d, V:%f\n", level>>2, (3.3 * ((float)(level>>2)/1023.0)));
    theDAC.setOutputLevel(level++);
    if (level==(1023<<2)) level=0;
    sleep_ms(10);
  }
  
  return 0;
}

