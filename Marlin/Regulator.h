#ifndef REGULATOR_H
#define REGULATOR_H

#include "Arduino.h"

/*================================================================================*/
/* Definitions  */
/*================================================================================*/
// Calculation for BITS_PER_PSI:
// R1 = 499
// R2 (input impedance of sensor) = 168k
// n = 12
// PSI_MAX = 100
// PSI_MIN = 2
// BITS_PER_PSI = (2^n/(PSI_MAX - PSI_MIN)) / (R2/(R1 + R2)) = 41.92
// Empirically tested at 43.76

#define BITS_PER_PSI    43.76

#define REG_OFFSET      1.7  // psi
#define REG_HYSTERESIS  0.0 // psi
#define MCP_CONST       (REG_OFFSET - REG_HYSTERESIS)
#define REGULATOR_LOW_P 2

/*================================================================================*/
/* Function Prototypes */
/*================================================================================*/
void setOutputPressure(float pressure);

#endif
