/**
 * MCodes.h - Interface for Mcodes
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/6/2016
 * Allows access to MCodes. Refactor from keeping them in Marlin_Main.cpp
 * Copyright (C) 2016 Voxel8
 */

#include "Marlin.h"
#include "MCodeUtilities.h"

/**
 * Set XYZE destination and feedrate from the current GCode command
 *
 *  - Set destination from included axis codes
 *  - Set to current for missing axis codes
 *  - Set the feedrate, if included
 */
void gcode_get_destination() {
  for (int i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i]))
      destination[i] = code_value() + (axis_relative_modes[i] || relative_mode ? current_position[i] : 0);
    else
      destination[i] = current_position[i];
  }
  if (code_seen('F')) {
    float next_feedrate = code_value();
    if (next_feedrate > 0.0) feedrate = next_feedrate;
  }
}

bool code_seen(char code) {
  seen_pointer = strchr(current_command_args, code);
  return (seen_pointer != NULL); // Return TRUE if the code-letter was found
}

float code_value() {
  float ret;
  char *e = strchr(seen_pointer, 'E');
  if (e) {
    *e = 0;
    ret = strtod(seen_pointer+1, NULL);
    *e = 'E';
  }
  else
    ret = strtod(seen_pointer+1, NULL);
  return ret;
}