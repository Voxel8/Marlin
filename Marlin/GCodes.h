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
 * M242 - General I2C Message Interface
 * M243 - Cartridge EEPROM Write
 * M244 - Cartridge EEPROM Read
 * M245 - Cartridge Diagnostics Readout
 * M247 - UV LED Enable/Disable
 * M249 - Enable / Disable Heated Bed Check
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
* M242 - General I2C Message Interface
*   A - 4 - 127 7-bit decimal device address
*   P - 0 - 255 Process ID (See I2C Commands in Voxel8_I2C_Commands.h)
*   D - 0 - 255 Data to write
*   E - 0 - 255 EEPROM address, if applicable
*/
inline void gcode_M242() {
  int verbose_level = code_seen('V') || code_seen('v') ? code_value_short() : 0;
  if (verbose_level < 0 || verbose_level > 4) {
    SERIAL_ECHOLNPGM("?(V)erbose Level is implausible (0-4).");
    return;
  }

  // Used to see if we've been given arguments, and to warn you through the
  // serial port if they're not seen.
  bool hasA, hasP, hasD, hasE;
  uint8_t i2c_address = 0xFF;
  uint8_t i2c_process_id = 0xFF;
  uint8_t i2c_eeprom_address = 0xFF;
  uint8_t i2c_data = 0xFF;

  // Desired address for peripheral device
  if (hasA = code_seen('A')) {
    i2c_address = (uint8_t)code_value();
  }
  // Desired process command given
  if (hasP = code_seen('P')) {
    i2c_process_id = (uint8_t)code_value();
  }
  // Desired data given
  if (hasD = code_seen('D')) {
    i2c_data = (uint8_t)code_value();
  }

  // EEPROM address, if applicable
  if (hasE = code_seen('E')) {
    i2c_eeprom_address = (uint8_t)code_value();
  }

  if (!hasA) {
    SERIAL_ECHOLNPGM("No peripheral address given");
    return;
  }
  if (!hasP) {
    SERIAL_ECHOLNPGM("No process command given");
    return;
  }
  if (!hasD) {
    SERIAL_ECHOLNPGM("No data given");
    return;
  }
  if (!hasE) {
    SERIAL_ECHOLNPGM("No eeprom address given");
  }

  I2C__GeneralCommand(i2c_address, i2c_process_id, i2c_eeprom_address,
                      i2c_data);
}

/*
* M243 - Cartridge EEPROM Write
*   C - 0 - 1   Cartridge Address (0 or 1)
*   E - 0 - 255 Cartridge EEPROM Address
*   D - 0 - 255 Value to write
*/
inline void gcode_M243() {
  uint8_t i2c_address = 0xFF;
  uint8_t i2c_data = 0xFF;
  uint8_t i2c_eeprom_address = 0xFF;

  // Used to see if we've been given arguments, and to warn you through the
  // serial port if they're not seen.
  bool hasC, hasE, hasD;

  // Desired address for peripheral device
  if (hasC = code_seen('C')) {
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
    }
  }
  // Desired EEPROM address
  if (hasE = code_seen('E')) {
    i2c_eeprom_address = (uint8_t)code_value();
    SERIAL_PROTOCOL(i2c_eeprom_address);
  }

  // Desired data to write
  if (hasD = code_seen('D')) {
    i2c_data = (uint8_t)code_value();
    SERIAL_PROTOCOL(i2c_data);
  }

  if (!hasC) {
    SERIAL_ECHOLNPGM("No addr given");
    return;
  }
  if (!hasD) {
    SERIAL_ECHOLNPGM("No data given");
    return;
  }
  if (!hasE) {
    SERIAL_ECHOLNPGM("No eeprom addr given");
    return;
  }

  I2C__EEPROMWrite(i2c_address, i2c_eeprom_address, i2c_data);
}

/*
* M244 - Cartridge EEPROM Read
*   C - 0 - 1   Cartridge Address (0 or 1)
*   E - 0 - 255 Cartridge EEPROM Address
*/
inline void gcode_M244() {
  uint8_t i2c_address = 0xFF;
  uint8_t i2c_eeprom_address = 0xFF;

  // Used to see if we've been given arguments, and to warn you through the
  // serial port if they're not seen.
  bool hasC, hasE;

  // Desired address for peripheral device
  if (hasC = code_seen('C')) {
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
    }
  }
  // Desired EEPROM address
  if (hasE = code_seen('E')) {
    i2c_eeprom_address = (uint8_t)code_value();
  }

  if (!hasC) {
    SERIAL_ECHOLNPGM("No cartridge address given");
    return;
  }

  if (!hasE) {
    SERIAL_ECHOLNPGM("No eeprom address given");
    return;
  }

  I2C__EEPROMRead(i2c_address, i2c_eeprom_address);
}

/*
* M245 - Cartridge Diagnostics Readout
*   C - 0 - 1   Cartridge Address (0 or 1)
*/
inline void gcode_M245() {
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

  I2C__GetSerial(i2c_address);
  I2C__GetProgrammerStation(i2c_address);
  I2C__GetCartridgeType(i2c_address);
  I2C__GetSize(i2c_address);
  I2C__GetMaterial(i2c_address);
  I2C__GetFirmwareVersion(i2c_address);
}

/**
* M247 - UV LED Enable/Disable
*   S - 0 - 255 value to send
*/
inline void gcode_M247() {
  int verbose_level = code_seen('V') || code_seen('v') ? code_value_short() : 0;
  if (verbose_level < 0 || verbose_level > 4) {
    SERIAL_ECHOLNPGM("?(V)erbose Level is implausible (0-4).");
    return;
  }

  uint8_t i2c_data;

  // Desired value given
  if (code_seen('S')) {
    i2c_data = code_value();
  }
  // TODO: add check whether S param is in bounds.

  I2C__ToggleUV(i2c_data);
}

/*
* M248 - Enable / Disable Cartridge Present Check
*   E - 0 - 1   1 = Enable, 0 = Disable
*/
inline void gcode_M248() {
  // Used to see if we've been given arguments, and to warn you through the
  // serial port if they're not seen.
  bool hasE;

  // Desired address for peripheral device
  if (hasE = code_seen('E')) {
    switch (int(code_value())) {
      case 0:
        Cartridge__SetPresentCheck(false);
        break;
      case 1:
        Cartridge__SetPresentCheck(true);
        break;
      default:
        SERIAL_ECHOLNPGM("Invalid Value");
        return;
    }
  }

  if (!hasE) {
    SERIAL_ECHOLNPGM("(E1) or (E0) not given");
    return;
  }
}

/*
* M249 - Enable / Disable Heated Bed Check
*   E - 0 - 1   1 = Enable, 0 = Disable
*/
inline void gcode_M249() {

  // Desired address for peripheral device
  if (code_seen('E')) {
    switch (int(code_value())) {
      case 0:
        HeatedBed__SetPresentCheck(false);
        break;
      case 1:
        HeatedBed__SetPresentCheck(true);
        break;
      default:
        SERIAL_ECHOLNPGM("Invalid Value");
        return;
    }
  } else {
    SERIAL_ECHOLNPGM("Enable (E1) or Disable (E0) not given");
    return;
  }
}

/*
* M251 - I2C Query Syringe Status, currently only valid for Cart 1
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
* M253 - I2C Query Vsense, currently only valid for cartridges
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
 * M272 - Set axis steps-per-unit for one or more axes, X, Y, Z, and E using
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
