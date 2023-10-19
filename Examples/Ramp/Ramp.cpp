#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../Src/MCP47X6.h"

// i2c1
#define I2C_BUS i2c1
const uint8_t sda = 6;
const uint8_t scl = 7;

// Instantiate objects used in this project
MCP47X6 theDAC=MCP47X6(i2c1, MCP47X6_DEFAULT_ADDRESS);

int main(void) {
  stdio_init_all();
  sleep_ms(1500);
  printf("Ramp\n");
  int pwr = 1;

  gpio_init(10);
  gpio_set_dir(10, GPIO_OUT);
  gpio_put(10, pwr);

  gpio_init(16);
  gpio_set_dir(16, GPIO_IN);
  gpio_pull_up(16);

  // setup
  i2c_init(I2C_BUS, 400 * 1000);
  gpio_pull_up(sda);
    gpio_pull_up(scl);
  bool status = theDAC.begin(sda, scl);
  printf("%s\n", (status ? "MCP47X6 connection successful" : "MCP47X6 connection failed"));
  //theDAC.begin(sda, scl, MCP47X6_VREF_VREFPIN & MCP47X6_GAIN_2X);
  theDAC.setVReference(MCP47X6_VREF_VREFPIN);
  theDAC.setGain(MCP47X6_GAIN_1X);
  theDAC.saveSettings();

  // loop
  uint16_t level = 0;
  char c;
  while (1) {
    printf("level: %d\n", level);
    theDAC.setOutputLevel(level++);
    if (level==(1023<<2)) level=0;
    sleep_ms(10);
    if (gpio_get(16)==0) {
      pwr = pwr ? 0 : 1;
      gpio_put(10, pwr);
    }
  }
  
  return 0;
}

