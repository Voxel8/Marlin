/**
 * GCodes.h - Contains codes for Gcodes.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/8/2016
 * Used as the home for Gcodes, to decouple them from Marlin_main. 
 * Note on the weirdness of having the definitions in an h file: This is to
 * facilitate the inline functions. Inline is only file specific, so doesn't
 * work if you count on the linker (ex, using the normal .cpp with an h file
 * interface). Directly including the code in an H file is the easiest way to
 * do this.
 * Copyright (C) 2016 Voxel8
 */

#ifndef G_CODES_H_
#define G_CODES_H_

#include "Marlin.h"
#include "GCodeUtility.h"
#include "planner.h"

//===========================================================================
//============================= Public Functions ============================
//===========================================================================


/**
 * M272: Set axis steps-per-unit for one or more axes, X, Y, Z, and E using
 *       the defualt ball-bar units
 *      (Follows the same syntax as G92)
 */
inline void gcode_M272(void) {
  for (int8_t i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      if (i == E_AXIS) {
        /* conversion from ball-bar units to um */
        float value = (213.3333333 / (1 + (code_value() / MM_TO_UM)));
        if (value < 20.0) {
          float factor =
              axis_steps_per_unit[i] /
              value;  // increase e constants if M92 E14 is given for netfab.
          max_e_jerk *= factor;
          max_feedrate[i] *= factor;
          axis_steps_per_sqr_second[i] *= factor;
        }
        axis_steps_per_unit[i] = value;
      } else {
        /* conversion from ball-bar units to um */
        float value = (213.3333333 / (1 + (code_value() / MM_TO_UM)));
        axis_steps_per_unit[i] = value;
      }
    }
  }
}

#endif  // G_CODES_H_
