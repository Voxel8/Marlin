/**
 * Voxel8_I2C_Commands.ch - Cartridge Holder / Cartridge I2C Commands.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 3/9/2016
 * Used to hold the I2C Commands Marlin can send to Voxel8's Cartridges and
 * Cartridge Holder.
 * Copyright (C) 2016 Voxel8
 */

#ifndef MARLIN_VOXEL8_I2C_COMMANDS_H_
#define MARLIN_VOXEL8_I2C_COMMANDS_H_

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

/* Cartridge and Cartridge Holder Settings/Commands */
// I2C Addresses
#define CART_HOLDER_ADDR        0x2F                  // Cartridge Holder
#define CART_PREFIX_ADDR        0x24
#define CART0_ADDR              CART_PREFIX_ADDR + 0  // Left Cartridge
#define CART1_ADDR              CART_PREFIX_ADDR + 1  // Right Cartridge

// Definitions for unused arguments for various commands
#define I2C_EMPTY_ADDRESS       0xFF
#define I2C_EMPTY_DATA          0xFF

// I2C Commands
#define SET_FAN_DRIVE_0_PWM     0x01
#define SET_LED_WHITE_0_PWM     0x02
#define SET_LED_WHITE_1_PWM     0x03
#define SET_LED_RED_0_PWM       0x04
#define SET_LED_UV_0_PWM        0x05
#define EEPROM_WRITE            0x06
#define EEPROM_READ             0x07
#define EEPROM_READ_SERIAL      0x08
#define EEPROM_READ_SIZE        0x09
#define EEPROM_READ_MTRL        0x10
#define EEPROM_READ_TYPE        0x11
#define EEPROM_READ_PRGMR       0x12

//===========================================================================
//============================= Public Functions ============================
//===========================================================================
/**
 * Sends a general I2C Command.
 * @parameter I2C_target_address  Address of the target (cartridge or 
 *                                cartridge holder)
 * @parameter command             The command being sent
 * @parameter address             The EEPROM address, if applicable
 * @parameter data                The data used for the command
 */ 
void I2C__GeneralCommand(uint8_t I2C_target_address, uint8_t command,
                         uint8_t address, uint8_t data);

/**
 * Sets the speed of the fan on the cartridge holder.
 * @parameter fanspeed            Speed the fan is set to
 */ 
void I2C__SetFanDrive0PWM(uint8_t fanSpeed);

/**
 * Turns off the fan on the cartridge holder.
 */ 
void I2C__SetFanOff(void);

/**
 * Writes data to a specific EEPROM address on a cartridge
 * @parameter cartridge           Address of the target (cartridge)
 * @parameter address             The EEPROM address being written to
 * @parameter data                The data being written
 */
void I2C__EEPROMWrite(uint8_t cartridge, uint8_t eeprom_address, uint8_t data);

/**
 * Reads data from a specific EEPROM address on a cartridge
 * @parameter cartridge           Address of the target (cartridge)
 * @parameter address             The EEPROM address being written to
 */ 
void I2C__EEPROMRead(uint8_t cartridge, uint8_t address);

/**
 * Read the serial number from a cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetSerial(uint8_t cartridge);

/**
 * Read the number of the programmer used on a cartridge and print it on
 * the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetProgrammerStation(uint8_t cartridge);

/**
 * Read the variety of cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetCartridgeType(uint8_t cartridge);

/**
 * Read the size of the nozzle from cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetSize(uint8_t cartridge);

/**
 * Read the material contained by a cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetMaterial(uint8_t cartridge);

#endif  // MARLIN_VOXEL8_I2C_COMMANDS_H_
