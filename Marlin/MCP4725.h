/***********************************************************************************
*
*   For use with Microchip MCP4725 12-bit DAC
*
***********************************************************************************/

#ifndef MCP4725_H
#define MCP4725_H

#include "Arduino.h"
#include <Wire.h>

/*================================================================================*/
/* I2C ADDRESS */
/*================================================================================*/
// First address byte is always (0 1 1 0)
// Second address is (A2 A1 A0 R/W). A2 and A1 are programmed by manufacturer per 
// customer request. If no request, both A2 and A1 are 0. Only A0 is configurable.
// Last bit is the read/~write bit

#define MCP4725_I2C_ADDRESS         0x60

/*================================================================================*/
/* READ/WRITE MODES */
/*================================================================================*/
// Last bit of address byte
#define MCP4725_READ                0x01
#define MCP4725_EEPROM_BUSY         0x80

/*================================================================================*/
/* POWER-DOWN MODES */
/*================================================================================*/
// Define modes for standard, full speed, fast mode, and high speed. This stuff 
// below is all for standard mode
#define MCP4725_NORMAL_MODE         0x00
#define MCP4725_POWER_DOWN_1K       0x02
#define MCP4725_POWER_DOWN_100K     0x04
#define MCP4725_POWER_DOWN_500K     0x06

/*================================================================================*/
/* WRITE COMMAND TYPES */
/*================================================================================*/
#define MCP4725_WRITE_FAST_MODE     0x00
#define MCP4725_WRITE_DAC_ONLY      0x40
#define MCP4725_WRITE_DAC_EEPROM    0x60

/*================================================================================*/
/* FUNCTION PROTOTYPES */
/*================================================================================*/
void DAC_write(uint8_t address, uint16_t data_val);

void DAC_write_EEPROM(uint8_t address, uint16_t data_val);

void DAC_read(uint8_t address);

void DAC_i2c_init(void);

#endif


/*      TO DO        */
/*-------------------*/

// Define Normal Mode, Fast Mode, High Speed mode (See datasheet 7.4)
// ^--Is fast mode (in write command) same as I2C fast mode?
