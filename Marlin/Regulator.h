#ifndef REGULATOR_H
#define REGULATOR_H

#include "Arduino.h"

/*================================================================================*/
/* Definitions  */
/*================================================================================*/
#define BITS_PER_PSI 33.5

/*================================================================================*/
/* Function Prototypes */
/*================================================================================*/
void setOutputPressure(float pressure);

#endif