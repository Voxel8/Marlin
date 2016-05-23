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
//================================ GCode List ===============================
//===========================================================================

/**
 * Look here for descriptions of G-codes:
 *  - http://linuxcnc.org/handbook/gcode/g-code.html
 *  - http://objects.reprap.org/wiki/Mendel_User_Manual:_RepRapGCodes
 *
 *
 * -----------------
 * Implemented Codes
 * -----------------
 *
 *
 * "M" Codes
 *
 * M251 - Queries small pneumatics cartridge to retrieve Syringe status
 * M253 - Queries cartridge to see if 24 volts are present or not.
          FFF: Returns whether hot end is active
          Pneumatics: Returns whether solenoid is active
 * M272 - Set axis steps-per-unit for one or more axes, X, Y, Z, and E using
 *        the default ball-bar units
*/

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

/*
* M251: I2C Query Syringe Status, currently only valid for Cart 1
*/
inline void gcode_M251() { I2C__GetGpioSwitch(CART1_ADDR); }

/*
* M253: I2C Query Vsense, currently only valid for cartridges
*   C - 0 - 1   Cartridge Address (0 or 1)
*/
inline void gcode_M253() {
  uint8_t i2c_address = 0xFF;

  // Desired address for peripheral device
  if (code_seen('C')) {
    switch (uint8_t(code_value())) {
      case 0:
        i2c_address = CART0_ADDR;
        break;
      case 1:
        i2c_address = CART1_ADDR;
        break;
      default:
        SERIAL_ECHOLNPGM("Invalid Cartridge Address");
        return;
    }
  } else {
    SERIAL_ECHOLNPGM("No cartridge address given");
    return;
  }

  I2C__GetVoltageSense(i2c_address);
}

/**
 * M272: Set axis steps-per-unit for one or more axes, X, Y, Z, and E using
 *       the default ball-bar units
 *       (Follows the same syntax as G92)
 */
inline void gcode_M272(void) {
  for (int8_t i = 0; i < NUM_AXIS; i++) {
    if (code_seen(axis_codes[i])) {
      if (i == E_AXIS) {
        /* conversion from ball-bar units to um */
        float converted_code_value =
            (213.3333333 / (1 + (code_value() / MM_TO_UM)));
        if (converted_code_value < 20.0) {
          float factor = axis_steps_per_unit[i] /
                         converted_code_value;  // increase e constants if M92
                                                // E14 is given for netfab.
          max_e_jerk *= factor;
          max_feedrate[i] *= factor;
          axis_steps_per_sqr_second[i] *= factor;
        }
        axis_steps_per_unit[i] = converted_code_value;
      } else {
        /* conversion from ball-bar units to um */
        float converted_code_value =
            (213.3333333 / (1 + (code_value() / MM_TO_UM)));
        axis_steps_per_unit[i] = converted_code_value;
      }
    }
  }
}

#endif  // G_CODES_H_
