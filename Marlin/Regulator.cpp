/***********************************************************************************
*
* Intended for use with MCP4725 12-bit DAC
*
***********************************************************************************/
#include "Regulator.h"
#include "MCP4725.h"

// Set the output pressure (in psi) of the regulator.
void setOutputPressure(float pressure) {
	// 4095/(130.355 psi)
	uint16_t digital_val = (uint16_t)(31.414215 * pressure);
	// Write value to DAC
	DAC_write(MCP4725_I2C_ADDRESS, digital_val);
}