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
//============================= Public Functions ============================
//===========================================================================

void I2C__GeneralCommand(uint8_t I2C_target_address,
                         uint8_t command,
                         uint8_t address,
                         uint8_t data);

void I2C__SetFanDrive0PWM(uint8_t fanSpeed);

void I2C__SetFanOff(void);

void I2C__EEPROMWrite(uint8_t cartridge,
                      uint8_t eeprom_address, 
                      uint8_t data);

uint8_t I2C__EEPROMRead(uint8_t cartridge, 
                             uint8_t address);

#endif  // MARLIN_VOXEL8_I2C_COMMANDS_H_