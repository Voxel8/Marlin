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
    #define REG_OFFSET      0.5   // psi
    #define REG_HYSTERESIS  0.2   // psi
#elif (E_REGULATOR_SENSOR == 2)   // Metalworks E-Reg
    #define BITS_PER_PSI    43.00
    #define REG_OFFSET      0.0   // psi
    #define REG_HYSTERESIS  0.0   // psi
#endif // E_REGULATOR_SENSOR
#define MCP_CONST       (REG_OFFSET - (REG_HYSTERESIS/2.0))
#define REGULATOR_LOW_P 2

/*================================================================================*/
/* Function Prototypes */
/*================================================================================*/
void setOutputPressure(float pressure);

#endif
