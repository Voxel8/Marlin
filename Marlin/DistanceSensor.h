/***********************************************************************************

For use with Laser Distance Sensor

@author Ricky Rininger - 4 September 2015

***********************************************************************************/
#include <stdint.h>
#include "Configuration.h"

#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#if EXT_ADC == 1

#define LINEAR_OFFSET	5.757    // linear coefficient of offset that was determined with empirical data
#define CONST_OFFSET    1.57     // constant term of offset
#define CONV_FACTOR     0.000188    // (6.144V / 32767 bits) 

#elif EXT_ADC == 2

#define LINEAR_OFFSET  	0		 // Constants not determined for this model
#define CONST_OFFSET 	0
#define CONV_FACTOR     0.003001    // (6.144V / 2047 bits) 

#endif

#define VOLT_TO_DIST    2000        // (10000 um / 5V)

#define LDIST_OFFSET 5000         // Offset range of the laser sensor
#define LDIST_UNIT_DIVISOR 1000   // Value to divide Z-Probe value by

/* Function Prototypes */
/*=====================*/

uint16_t get_dist_SingleEnded(uint8_t channel);

uint16_t get_dist_Differential(uint8_t first_channel, uint8_t second_channel);

#endif
