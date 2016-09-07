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
 * M252 - Clear error state on I2C peripherals (cartridges and holder)
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
* M250: Output whether cartridge is present over serial connection
*   Usage: M250 T0 or M250 T1 to query presence of cartridge 0 or 1,
*   respectively.
*/
inline void gcode_M250() {
  if (code_seen('T')) {
    switch ((uint8_t)code_value()) {
      case 0:
        if (Cartridge__Present(0)) {
          SERIAL_ECHOLNPGM("true");
        }
        else {
          SERIAL_ECHOLNPGM("false");
        }
        break;
      case 1:
        if (Cartridge__Present(1)) {
          SERIAL_ECHOLNPGM("true");
        }
        else {
          SERIAL_ECHOLNPGM("false");
        }
        break;
      default:
        SERIAL_ECHOLNPGM("Invalid tool number!")
    } // end switch
  }
  else {
    SERIAL_ECHOLNPGM("No tool number given");
  }
}


/*
* M251: I2C Query Syringe Status, currently only valid for Cart 1
*/
inline void gcode_M251() { I2C__GetGpioSwitch(CART1_ADDR); }

/*
* M252 - Clear error state on I2C peripherals (cartridges and holder)
*   C - 0 - 2   0 = FFF 1 = Pneumatics 2 = Cartridge Holder
*/
inline void gcode_M252() {
  uint8_t i2c_address = 0xFF;

  // Desired address for peripheral device
  if (code_seen('C')) {
    switch (int(code_value())) {
      case 0:
        i2c_address = CART0_ADDR;
        break;
      case 1:
        i2c_address = CART1_ADDR;
        break;
      case 2:
        i2c_address = CART_HOLDER_ADDR;
        break;
      default:
        SERIAL_ECHOLNPGM("Invalid Address");
        return;
    }
  } else {
    SERIAL_ECHOLNPGM("No cartridge address given");
    return;
  }

  I2C__ClearError(i2c_address);
}

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

/**
 * M277: Enable/disable auger extrusion (E0)
 */
inline void gcode_M277() {
  if (code_seen('S')) {
    switch(uint8_t(code_value())) {
      case 0:
        Cartridge__SetAugerEnabled(0);
        break;
      case 255:
        Cartridge__SetAugerEnabled(1);
        break;
      default:
        SERIAL_PROTOCOLLNPGM("Invalid code given");
    }
  } else {
    Cartridge__SetAugerEnabled(1);
  }
}

/*
* M248 - Enable / Disable Protections
*   S - 0 - 2   0 = Cartridge Check, 1 = Heated Bed Check, 2 = Pressure 
*               Protections Check
*   E - 0 - 1   1 = Enable, 0 = Disable
*/
inline void gcode_M248() {
  // Used to see if we've been given arguments, and to warn you through the
  // serial port if they're not seen.
  bool hasS;
  bool hasE;

  if (hasS = code_seen('S')) {
    switch(int(code_value())) {
      case 0:
      // Cartridge Check
        if (hasE = code_seen('E')) {
          switch(int(code_value())) {
            case 0:
              Cartridge__SetPresentCheck(false);
              break;
            case 1:
              Cartridge__SetPresentCheck(true);
              break;
            default:
              SERIAL_PROTOCOLLNPGM("(E1) or (E0) not given");
              return;
          }
        }
        break;
      case 1:
      // Heated Bed Check
        if (hasE = code_seen('E')) {
          switch(int(code_value())) {
            case 0:
              HeatedBed__SetPresentCheck(false);
              break;
            case 1:
              HeatedBed__SetPresentCheck(true);
              break;
            default:
              SERIAL_PROTOCOLLNPGM("(E1) or (E0) not given");
              return;
          }
        }
        break;
      case 2:
      // Regulator Protection
        if (hasE = code_seen('E')) {
          switch(int(code_value())) {
            case 0:
              Regulator__SetPressureProtections(false);
              break;
            case 1:
              Regulator__SetPressureProtections(true);
              break;
            default:
              SERIAL_PROTOCOLLNPGM("(E1) or (E0) not given");
              return;
          }
        }
        break;
      default:
        SERIAL_PROTOCOLLNPGM(
            "Invalid S argument: S0 = Cartridge, S1 = HeatedBed, S2 = "
            "Pneumatics");
        return;
    }
  }

  if (!hasS) {
    SERIAL_PROTOCOLLNPGM(
        "No S argument given: S0 = Cartridge, S1 = HeatedBed, S2 = "
        "Pneumatics");
    }
    if (!hasE) {
      SERIAL_PROTOCOLLNPGM("(E1) or (E0) not given");
    }
}

#endif  // G_CODES_H_