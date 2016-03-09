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


/*
* M242 - General I2C Message Interface
*   A - 4 - 127 7-bit decimal device address
*   P - 0 - 255 Process ID (See I2C Commands in Configuration_adv.h)
*   S - 0 - 255 value to send
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

void I2C__SetFanDrive0PWM(uint8_t fanSpeed){

    // Check fanspeed, cap at MAX_FAN_DUTY
    if (fanSpeed > MAX_FAN_DUTY){
    	fanSpeed = MAX_FAN_DUTY;
    }

    // Send message
	writeThreeBytePacket(CART_HOLDER_ADDR, SET_FAN_DRIVE_0_PWM, fanSpeed,
	                	 I2C_EMPTY_ADDRESS);

	// Send information to Octoprint
	#if defined(DEBUG)
      SERIAL_PROTOCOLLNPGM("Command: 'Set Fan Speed' Sent");
      SERIAL_PROTOCOL("fanSpeed = ");
      SERIAL_PROTOCOL(fanSpeed);
    #endif // end DEBUG
}

void I2C__SetFanOff(void){
	uint8_t fanSpeed = 0;

	// Send message
	writeThreeBytePacket(CART_HOLDER_ADDR, SET_FAN_DRIVE_0_PWM,fanSpeed, 
		                 I2C_EMPTY_ADDRESS);

    // Send information to Octoprint
    #if defined(DEBUG)
      SERIAL_PROTOCOLLNPGM("Command: 'Fan Off' Sent");
      SERIAL_PROTOCOL("fanSpeed = ");
      SERIAL_PROTOCOL(fanSpeed);
    #endif // end DEBUG
}

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

uint8_t I2C__EEPROMRead(uint8_t cartridge, 
	                    uint8_t address){
	// Send message
	writeThreeBytePacket(cartridge, EEPROM_READ, address, I2C_EMPTY_DATA);

	// Send information to Octoprint
	SERIAL_PROTOCOLLNPGM("Command: 'EEPROM READ' Sent");
	SERIAL_PROTOCOL("Value = ");

	// Read from cartridge and report
	requestAndPrintPacket(cartridge, 1);
}


//===========================================================================
//============================ Private Functions ============================
//===========================================================================

void writeThreeBytePacket(uint8_t I2C_target_address, 
	                      uint8_t command,
						  uint8_t address,
						  uint8_t data){
	Wire.beginTransmission((uint8_t)I2C_target_address);
	Wire.write(command);
	Wire.write(address);
    Wire.write(data);
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