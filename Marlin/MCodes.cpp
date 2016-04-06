/**
 * MCodes.h - Interface for Mcodes
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/6/2016
 * Allows access to MCodes. Refactor from keeping them in Marlin_Main.cpp
 * Copyright (C) 2016 Voxel8
 */

#include "Marlin.h"
#include "MCodes.h"
#include "MCodeUtilities.h"

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
inline void gcode_G0_G1() {
  if (IsRunning()) {
    gcode_get_destination();  // For X Y Z E F

    #if ENABLED(FWRETRACT)

    if (autoretract_enabled &&
        !(code_seen('X') || code_seen('Y') || code_seen('Z')) &&
        code_seen('E')) {
      float echange = destination[E_AXIS] - current_position[E_AXIS];
      // Is this move an attempt to retract or recover?
      if ((echange < -MIN_RETRACT && !retracted[active_extruder]) ||
          (echange > MIN_RETRACT && retracted[active_extruder])) {
        current_position[E_AXIS] =
            destination[E_AXIS];  // hide the slicer-generated retract/recover
                                  // from calculations
        plan_set_e_position(current_position[E_AXIS]);  // AND from the planner
        retract(!retracted[active_extruder]);
        return;
      }
    }

    #endif  // FWRETRACT

    prepare_move();
  }
}