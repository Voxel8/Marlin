#ifndef REGULATOR_H
#define REGULATOR_H

#include "Arduino.h"
#include "Configuration.h"

/*================================================================================*/
/* Definitions  */
/*================================================================================*/

// Relevant Pins
// Voxel8 C2
// #define PNEUMATIC_PUMP_PIN    7   // Pump driver
// #define PNEUMATIC_PIN         5   // A5 Input analog tank pressure reading
// #define PNEUMATIC_CART1_PIN   4   // A4 Cartridge 2 analog pressure reading

// Calculation for BITS_PER_PSI:
// R1 = 499
// n = 12
// Metalworks E-Reg Settings:
// R2 (input impedance of sensor) = 168k
// Type of input: 0/5 V
// Measure unit: psi
// PSI_MAX = 100
// PSI_MIN = 0
// Speed adjust: 4
// Filter analog input: 2
// 
// BITS_PER_PSI = (2^n/(PSI_MAX - PSI_MIN)) / (R2/(R1 + R2)) = 40.92
// Empirically tuned to 43.00

#if (E_REGULATOR_SENSOR == 1)     // SMC E-Reg
    #define BITS_PER_PSI    33.1
    #define REG_OFFSET      0.25  // psi
    #define REG_HYSTERESIS  0.2   // psi
#elif (E_REGULATOR_SENSOR == 2)   // Metalworks E-Reg
    #define BITS_PER_PSI    43.00
    #define REG_OFFSET      0.0   // psi
    #define REG_HYSTERESIS  0.0   // psi
#endif // E_REGULATOR_SENSOR
#define REGULATOR_LOW_P 2

#define REGULATOR_PROTECTION_TIME_S (5)
#define REGULATOR_PROTECTION_BAND   (4)

/*================================================================================*/
/* Function Prototypes */
/*================================================================================*/

/**
 * Set the output pressure (in psi) of the regulator.
 * @input desired_pressure  The pressure that the function is attempting to 
 *                          reach.
 */
void Regulator__SetOutputPressure(float pressure);

/**
 * Updates the protection function, called regularly in the main loop.
 */
void Regulator__Update();

#endif
