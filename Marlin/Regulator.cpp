/***********************************************************************************
*
* Intended for use with MCP4725 12-bit DAC
*
***********************************************************************************/
#include "Regulator.h"
#include "MCP4725.h"


// Set the output pressure (in psi) of the regulator.
void setOutputPressure(float pressure) {
	// 4095/(130.355 psi) (adjusted from theoretical value for better accuracy)
	uint16_t digital_val = (uint16_t)(BITS_PER_PSI * (pressure + MCP_CONST));
	// Write value to DAC
	DAC_write(MCP4725_I2C_ADDRESS, digital_val);
}
// TO DO
//-------
// Add else if for M236, for the case when 'S' isn't provided. Perhaps return current output pressure?
