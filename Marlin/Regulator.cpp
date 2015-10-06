/***********************************************************************************
*
* Intended for use with MCP4725 12-bit DAC
*
***********************************************************************************/
#include "Regulator.h"
#include "MCP4725.h"
#include "temperature.h"

// Set the output pressure (in psi) of the regulator.
void setOutputPressure(float desired_pressure) {
    uint16_t digital_val = 0;
    // Increasing pressure
    if(desired_pressure > pressureRegulator()) {
        // Add half of hysteresis range to desired pressure
        desired_pressure += (REG_HYSTERESIS/2.0);
    }
    // Decreasing pressure
    else if(desired_pressure < pressureRegulator()) {
        // Subtract half of hysteresis range from desired pressure
        desired_pressure -= (REG_HYSTERESIS/2.0);
    }

	digital_val = (uint16_t)(BITS_PER_PSI * (desired_pressure + MCP_CONST));
	// Write value to DAC
	DAC_write(MCP4725_I2C_ADDRESS, digital_val);
}
