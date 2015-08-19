#include "Configuration.h"
#include "MCP4725.h"

#include "Arduino.h"
#include <Wire.h>

/*================================================================================*/
/* DAC Write Function */
/*================================================================================*/
// normal/standard mode for now
void DAC_write(uint8_t address, uint16_t data_val) {
	// Normal 
	uint8_t	config = (MCP4725_WRITE_DAC_ONLY | MCP4725_NORMAL_MODE);


	uint8_t byte_a = data_val >> 4;
	uint8_t byte_b = (data_val & 0x0F) << 4;

	Wire.beginTransmission(address);
	Wire.write(config);
	Wire.write(byte_a);
	Wire.write(byte_b);
	Wire.endTransmission();
	// have this whole function return the value of endTransmission, then, depending
	// on the return value, send an appropriate message to the serial console.
	// Make this feature selectable so it can be disabled.
	// result = Wire.endTransmission();
	
	// switch(result) {
	// 	case 0:
	// 		break;
	// 	case 1:
	// 		break;
	// 	case 2:
	// 		break;
	// 	case 3:
	// 		break;
	// 	case 4:
	// 		break;
	// // Put this switch statement in Marlin main (FIX THIS SHIT 19/08/2015) TESTING
	// }
	// return result;
}

/*================================================================================*/
/* DAC Write Function with EEPROM Write*/
/*================================================================================*/
void DAC_write_EEPROM(uint8_t address, uint16_t data_val) {
	// EEPROM write mode, normal power mode
	uint8_t	config = (MCP4725_WRITE_DAC_EEPROM | MCP4725_NORMAL_MODE);

	// Break data_val down into bytes
	uint8_t byte_a = data_val >> 4;
	uint8_t byte_b = (data_val & 0x0F) << 4;

	// Send the data over I2C
	Wire.beginTransmission(address);
	Wire.write(config);
	Wire.write(byte_a);
	Wire.write(byte_b);
	Wire.endTransmission();
	// result = Wire.endTransmission();

	// switch(result) {
	// 	case 0:
	// 		break;
	// 	case 1:
	// 		break;
	// 	case 2:
	// 		break;
	// 	case 3:
	// 		break;
	// 	case 4:
	// 		break;
	// 		// ALSO put THIS shit in a diff function. Holy Hell... TESTING
	// }
	// return result;
}

/*================================================================================*/
/* DAC Read Function (read value in DAC EEPROM) */
/*================================================================================*/
void DAC_read(uint8_t address) {
	// Add read bit to address (LSB)
	uint8_t address_modified = (address | MCP4725_READ); 

	// This function is not done! at all!
	// Probably want to actually return a value...
}

/*================================================================================*/
/* DAC I2C initialization */
/*================================================================================*/
void DAC_i2c_init(void) {
	Wire.begin();
	// Maybe set some defined inital output?
}

// TO DO
//----------
// define initial output?
// conversion function or something to convert pressure to voltage? (where?)
// >	Mcode M236 should take pressure as an argument 
