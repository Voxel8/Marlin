/**
 * GCodes.cpp - Contains codes for Gcodes.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/8/2016
 * Contains code for individual GCodes.
 * Copyright (C) 2016 Voxel8
 */

#include "Marlin.h"
#include "GCodes.h"
#include "GCodeUtility.h"
#include "planner.h"

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

inline void gcode_M272(void) {
  for (int8_t i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      if (i == E_AXIS) {
        float value = (213.3333333 / (1 + (code_value() / 1000)));
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
        axis_steps_per_unit[i] = code_value();
      }
    }
  }
}