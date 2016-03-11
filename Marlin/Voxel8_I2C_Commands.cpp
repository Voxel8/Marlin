/**
 * Voxel8_I2C_Commands.cpp - Cartridge Holder / Cartridge I2C Commands.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 3/9/2016
 * Used to hold the I2C Commands Marlin can send to Voxel8's Cartridges and
 * Cartridge Holder.
 * Copyright (C) 2016 Voxel8
 */

#include "Marlin.h"
#include "Voxel8_I2C_Commands.h"
#include <Wire.h>

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

// Defines for specific commands 
// 127 (50%) is maxy duty cycle for 12V fans
#define MAX_FAN_DUTY  127
//===========================================================================
//============================ Private Variables ============================
//===========================================================================

//===========================================================================
//====================== Private Functions Prototypes =======================
//===========================================================================
void writeThreeBytePacket(uint8_t I2C_target_address, 
                          uint8_t command,
                          uint8_t address,
                          uint8_t data);

void requestAndPrintPacket(uint8_t I2C_target_address,
                           uint8_t bytes);
//===========================================================================
//============================ Public Functions =============================
//===========================================================================


/**
 * Sends a general I2C Command.
 * @parameter I2C_target_address  Address of the target (cartridge or 
 *                                cartridge holder)
 * @parameter command             The command being sent
 * @parameter address             The EEPROM address, if applicable
 * @parameter data                The data used for the command
 */ 
void I2C__GeneralCommand(uint8_t I2C_target_address,
                         uint8_t command,
                         uint8_t address,
                         uint8_t data) {
  writeThreeBytePacket(I2C_target_address, command, address, data);

  #if defined(DEBUG)
    SERIAL_PROTOCOLLNPGM("Command: 'I2C Command' Sent");
    SERIAL_PROTOCOL("Target address = ");
    SERIAL_PROTOCOL(I2C_target_address);
    SERIAL_EOL;
    SERIAL_PROTOCOL("command = ");
    SERIAL_PROTOCOL(command);
    SERIAL_EOL;
    SERIAL_PROTOCOL("address = ");
    SERIAL_PROTOCOL(address);
    SERIAL_EOL;
    SERIAL_PROTOCOL("data = ");
    SERIAL_PROTOCOL(data);
    SERIAL_EOL;
  #endif // end DEBUG
}

/**
 * Sets the speed of the fan on the cartridge holder.
 * @parameter fanspeed            Speed the fan is set to
 */ 
void I2C__SetFanDrive0PWM(uint8_t fanSpeed){

    // Check fanspeed, cap at MAX_FAN_DUTY
    if (fanSpeed > MAX_FAN_DUTY){
        fanSpeed = MAX_FAN_DUTY;
    }

    // Send message
    writeThreeBytePacket(CART_HOLDER_ADDR, SET_FAN_DRIVE_0_PWM,
                         I2C_EMPTY_ADDRESS, fanSpeed);

    // Send information to Octoprint
    #if defined(DEBUG)
      SERIAL_PROTOCOLLNPGM("Command: 'Set Fan Speed' Sent");
      SERIAL_PROTOCOL("fanSpeed = ");
      SERIAL_PROTOCOL(fanSpeed);
    #endif // end DEBUG
}

/**
 * Turns off the fan on the cartridge holder.
 */ 
void I2C__SetFanOff(void){
    uint8_t fanSpeed = 0;

    // Send message
    writeThreeBytePacket(CART_HOLDER_ADDR, SET_FAN_DRIVE_0_PWM,
                         I2C_EMPTY_ADDRESS, fanSpeed);

    // Send information to Octoprint
    #if defined(DEBUG)
      SERIAL_PROTOCOLLNPGM("Command: 'Fan Off' Sent");
      SERIAL_PROTOCOL("fanSpeed = ");
      SERIAL_PROTOCOL(fanSpeed);
    #endif // end DEBUG
}

/**
 * Writes data to a specific EEPROM address on a cartridge
 * @parameter cartridge           Address of the target (cartridge)
 * @parameter address             The EEPROM address being written to
 * @parameter data                The data being written
 */
void I2C__EEPROMWrite(uint8_t cartridge,
                      uint8_t eeprom_address, 
                      uint8_t data){
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_WRITE, eeprom_address, data);

    // Send information to Octoprint
    SERIAL_PROTOCOLLNPGM("Command: 'EEPROM Write' Sent");
    SERIAL_PROTOCOL("Value = ");
    SERIAL_PROTOCOL(data);
    SERIAL_PROTOCOL("Address = ");
    SERIAL_PROTOCOL(eeprom_address);

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 1);
}

/**
 * Reads data from a specific EEPROM address on a cartridge
 * @parameter cartridge           Address of the target (cartridge)
 * @parameter address             The EEPROM address being written to
 */ 
void I2C__EEPROMRead(uint8_t cartridge, 
                     uint8_t address) {
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_READ, address, I2C_EMPTY_DATA);

    // Send information to Octoprint
    SERIAL_PROTOCOLLNPGM("Command: 'EEPROM READ' Sent");
    SERIAL_PROTOCOL("Value = ");

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 1);
}

/**
 * Read the serial number from a cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetSerial(uint8_t cartridge) {
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_READ_SERIAL, I2C_EMPTY_ADDRESS, 
                         I2C_EMPTY_DATA);

    // Send information to Octoprint
    SERIAL_PROTOCOL("Serial Number = ");

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 2);
}

/**
 * Read the number of the programmer used on a cartridge and print it on
 * the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetProgrammerStation(uint8_t cartridge) {
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_READ_PRGMR, I2C_EMPTY_ADDRESS, 
                         I2C_EMPTY_DATA);

    // Send information to Octoprint
    SERIAL_PROTOCOL("Programmer Station = ");

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 1);
}

/**
 * Read the variety of cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetCartridgeType(uint8_t cartridge) {
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_READ_TYPE, I2C_EMPTY_ADDRESS, 
                         I2C_EMPTY_DATA);

    // Send information to Octoprint
    SERIAL_PROTOCOL("Cartridge Type = ");

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 1);
}

/**
 * Read the size of the nozzle from cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetSize(uint8_t cartridge) {
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_READ_SIZE, I2C_EMPTY_ADDRESS, 
                         I2C_EMPTY_DATA);

    // Send information to Octoprint
    SERIAL_PROTOCOL("Cartridge Size = ");

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 1);
}

/**
 * Read the material contained by a cartridge and print it on the serial port
 * @parameter cartridge           Address of the target (cartridge)
 */ 
void I2C__GetMaterial(uint8_t cartridge) {
    // Send message
    writeThreeBytePacket(cartridge, EEPROM_READ_MTRL, I2C_EMPTY_ADDRESS, 
                         I2C_EMPTY_DATA);

    // Send information to Octoprint
    SERIAL_PROTOCOL("Cartridge Material = ");

    // Read from cartridge and report
    requestAndPrintPacket(cartridge, 1);
}

//===========================================================================
//============================ Private Functions ============================
//===========================================================================

/*
 * DEFINES ON CARTRIDGES, USE THIS AS A REFERENCE FOR HOW TO WRITE YOUR PACKETS
 * #define I2C_BFFR_SIZE       0x03    // 2 bytes, command, address, and data
 * #define I2C_COMMAND         0
 * #define I2C_DATA            1
 * #define I2C_ADDRESS         2
*/

void writeThreeBytePacket(uint8_t I2C_target_address, 
                          uint8_t command,
                          uint8_t address,
                          uint8_t data){
    Wire.beginTransmission((uint8_t)I2C_target_address);
    Wire.write(command);
    Wire.write(data);
    Wire.write(address);
    Wire.endTransmission();
}

void requestAndPrintPacket(uint8_t I2C_target_address,
                           uint8_t bytes){
    // Read from cartridge and report
    Wire.requestFrom(I2C_target_address,bytes);
    while (Wire.available()){
        SERIAL_PROTOCOL(Wire.read());
    } 
    SERIAL_EOL;
}