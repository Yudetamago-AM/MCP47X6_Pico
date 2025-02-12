/**************************************************************************/
/*! 
    @file     MCP47X6.cpp
    @author   C. Schnarel
    @author   Yudetamago-Am
	@license  BSD (see license.txt)
	
    This file is part of an Raspberry Pi Pico C/C++ SDK library to interface with the
    Microchip MCP47X6 series of Digital-to-Analog converters which are
    connected via the I2C bus.

    This file is the MCP47X6 I2C device class file
    Based on Microchip datasheets for the following part numbers
        MCP4706 (8-bit), MCP4716 (10-bit), MCP4726 (12-bit)
    These parts share a common programming interface

    Copyright (c) 2013 Chip Schnarel <schnarel@hotmail.com>
    Copyright (c) 2023 Yudetamago-AM <akiom@rittai.org>

    Original Library: Updates should (hopefully) always be available at
        https://github.com/uchip/MCP47X6

    Forked(this) Library:
        https://github.com/Yudetamago-AM/MCP47X6_Pico

	@section  HISTORY

    2013-Dec-25  - First release, C. Schnarel
    2023-Oct-11  - Modify to RP C/C++ SDK, Yudetamago-AM
*/
/**************************************************************************/

#include "MCP47X6.h"
//#include "Wire.h"

/**************************************************************************/

/******************************************
 * Default constructor, uses default I2C address.
 * @see MCP47X6_DEFAULT_ADDRESS
 */
MCP47X6::MCP47X6(i2c_inst_t* i2c) {
    this->i2c = i2c;
    devAddr = MCP47X6_DEFAULT_ADDRESS;
}

/******************************************
 * Specific address constructor.
 * @param address I2C address
 * @see MCP47X6_DEFAULT_ADDRESS
 */
MCP47X6::MCP47X6(i2c_inst_t* i2c, uint8_t address) {
    this->i2c = i2c;
    devAddr = address;
}

/******************************************
 * Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return true if connection is valid, false otherwise
 */
#if 0
bool MCP47X6::testConnection(void) {
    
    Wire.beginTransmission(devAddr);
    return (Wire.endTransmission() == 0);
  
}
#endif

/******************************************
 * Power on and prepare for general usage.
 * This device recalls the previously saved state at power up. The begin()
 * function reads these values from the DAC and uses them to reinitialize
 * the driver since a reset of the microcontroller
 * did not necessarily restart the device.
 */
bool MCP47X6::begin(uint8_t sda, uint8_t scl) {
  // read the settings from DAC EEPROM

  // reinitialize the device from the read settings
  return writeConfigReg(config);
}

bool MCP47X6::begin(uint8_t sda, uint8_t scl, uint8_t newConfig) {
  // initialize the device from the new settings
  config = newConfig;
  return true;
}

/******************************************
 * Set the configuration bits for the DAC
 */
void MCP47X6::setGain(uint8_t gain) {
  config = (config & MCP47X6_GAIN_MASK) | (gain & !MCP47X6_GAIN_MASK);
}

void MCP47X6::setVReference(uint8_t vref) {
  config = (config & MCP47X6_VREF_MASK) | (vref & !MCP47X6_VREF_MASK);
}

//void MCP47X6::setPwrDnOutRes(uint8_t pdOutR) {
//  config = (config & MCP47X6_PWRDN_MASK) | (pdOutR & !MCP47X6_PWRDN_MASK);
//}

/******************************************
 * Saves current DAC settings into DAC EEPROM for use at power up
 */
bool MCP47X6::saveSettings(void) {
  // read the current volatile settings
  // write the values back to DAC EEPROM
  return writeConfigReg(config);
}

/******************************************
 * Set DAC output level value
 * Valid range is 0x000 to 0xFFF for all parts.
 * For the MCP4726 all 4096 steps are used.
 * For the MCP4716 only 1024 steps are used. The two LSBs are not used.
 * (i.e. value & 0xFFC)
 * For the MCP4706 only 256 steps are used. The four LSBs are not used.
 * (i.e. value & 0xFF0)
 */
bool MCP47X6::setOutputLevel(uint16_t level) {
  uint8_t buf[3];
  buf[0] = ((config | MCP47X6_CMD_VOLALL) & MCP47X6_PWRDN_MASK);
  buf[1] = ((uint8_t)((level>>4) & 0xFF));
  buf[2] = ((uint8_t)((level<<4) & 0xF0));

  int8_t ret = i2c_write_blocking(i2c, devAddr, buf, 3, false);
  return (ret!=PICO_ERROR_GENERIC);
}

// Special case for 8-bit device (MCP4706) - saves one byte of transfer
// and is therefore faster
bool MCP47X6::setOutputLevel(uint8_t level) {
  uint8_t buf[2];
  buf[0] = ((uint8_t)MCP47X6_CMD_VOLDAC);
  buf[1] = level;

  int8_t ret = i2c_write_blocking(i2c, devAddr, buf, 2, false);
  return (ret!=PICO_ERROR_GENERIC);
}


/******************************************
 * Put the DAC into a low power state
 * NOTE: writing any settings or DAC output level value
 * returns the DAC to the awake power state.
 */
bool MCP47X6::powerDown() {
  return writeConfigReg(config);
}

bool MCP47X6::powerDown(uint8_t pdOutR) {
  config = (config & MCP47X6_PWRDN_MASK) | (pdOutR & !MCP47X6_PWRDN_MASK);
  return writeConfigReg(config);
}

/******************************************
 * Private helper function to write just the config register
 */
bool MCP47X6::writeConfigReg(uint8_t theConfig) {
  uint8_t buf;
  buf = (theConfig | MCP47X6_CMD_VOLCONFIG);

  int8_t ret = i2c_write_blocking(i2c, devAddr, &buf, 1, false);
  return (ret!=PICO_ERROR_GENERIC);
}