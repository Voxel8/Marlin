/*


*/
/*================================================================================*/
/*                            GET DISTANCE (Single-Ended)                         */
/*================================================================================*/

#include "DistanceSensor.h"
#include "ADC.h"

uint16_t get_dist_SingleEnded(uint8_t channel) {

	float distance = 0;
    uint16_t val_raw = 0;

    val_raw = readADC_SingleEnded(channel);

    // THEORETICALLY, the formula for distance is distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST)
    // Empirical data has shown that a linear offset exists such that ...
    // distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - offset  , or
    // distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - (mx + b)
    // where m = LINEAR_OFFSET, x = (val_raw * CONV_FACTOR), and b = CONST_OFFSET

    distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - ((LINEAR_OFFSET * val_raw * CONV_FACTOR) - CONST_OFFSET);
    
    return distance;
}
/*================================================================================*/
/*                            GET DISTANCE (Differential)                         */
/*================================================================================*/

uint16_t get_dist_Differential(uint8_t first_channel, uint8_t second_channel) {

	float distance = 0;
    uint16_t val_raw = 0;

    val_raw = readADC_Differential(first_channel, second_channel);

    // THEORETICALLY, the formula for distance is distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST)
    // Empirical data has shown that a linear offset exists such that ...
    // distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - offset  , or
    // distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - (mx + b)
    // where m = LINEAR_OFFSET, x = (val_raw * CONV_FACTOR), and b = CONST_OFFSET

    distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - ((LINEAR_OFFSET * val_raw * CONV_FACTOR) - CONST_OFFSET);
    
    return distance;
}
