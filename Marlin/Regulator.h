#ifndef REGULATOR_H
#define REGULATOR_H

#include "Arduino.h"

/*================================================================================*/
/* Definitions  */
/*================================================================================*/
// Calculation for BITS_PER_PSI:
// R1 = 499
// R2 (input impedance of sensor) = 10k
// n = 12
// BITS_PER_PSI = (2^n/OUTPUT_PSI_MAX) / (R2/(R1 + R2)) approx = 33.1

#define BITS_PER_PSI    33.1

#define REG_OFFSET      0.5 // psi
#define REG_HYSTERESIS  0.2 // psi
#define MCP_CONST       (REG_OFFSET - (REG_HYSTERESIS/2.0))
#define REGULATOR_LOW_P 2

/*================================================================================*/
/* Function Prototypes */
/*================================================================================*/
void setOutputPressure(float pressure);

#endif
