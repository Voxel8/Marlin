#ifndef REGULATOR_H
#define REGULATOR_H

#include "Arduino.h"
#include "Configuration.h"

/*================================================================================*/
/* Definitions  */
/*================================================================================*/

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

// The amount of time the pressure must be outside the pressure bounds to activate
// the protection
#define REGULATOR_PROTECTION_TIME_S    (5)

// The regulator protection is divided into two bands, with lower PSI having a 
// tighter tolerance on the acceptable pressure values. The crossover is set below,
// and the plus / minus values acceptable are set with REGULATOR_PROTECTION_BAND_LOW
// and REGULATOR_PROTECTION_BAND_HIGH. REGULATOR_NOT_PRESENT_VALUE attempts to 
// identify when the system is running without a pneumatics sled present, and
// REGULATOR_LOW_P allows the pressure to have greater resolution near 0 PSI.
#define REGULATOR_BAND_CROSSOVER_PSI   (HOUSE_AIR_THRESH)
#define REGULATOR_PROTECTION_BAND_LOW  (5)
#define REGULATOR_PROTECTION_BAND_HIGH (10)
#define REGULATOR_NOT_PRESENT_VALUE    (110)
#define REGULATOR_LOW_P                (2)

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

 /** 
  * Enables or disables pressure protections
  * @value     true = enable, false = no check.
  */
  void Regulator__SetPressureProtections(bool value);

#endif
