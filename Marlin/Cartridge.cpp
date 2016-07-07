/**
 * Cartridge.cpp - Used to keep track of the status of the cartridges.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 2/19/2016
 * Used to keep track of all information about material cartridges, for use
 * in decision making in other parts of the program
 * Copyright (C) 2016 Voxel8
 */

#include "Marlin.h"
#include "Cartridge.h"

#include "./temperature.h"  // for disable_all_heaters()
#include "./stepper.h"      // for quickStop()
#include "./language.h"     // for MSG_T_CARTRIDGE_REMOVED

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

#define NUMBER_OF_CARTRIDGES (2)
#define CARTRIDGE_REMOVAL_HYSTERESIS_COUNT (250)
#define FFF_INDEX (0)
#define SILVER_INDEX (1)

typedef enum _CartridgeStatus { PRESENT, ABSENT, REMOVED } CARTRIDGE_STATUS;

//===========================================================================
//============================ Private Variables ============================
//===========================================================================

static CARTRIDGE_STATUS cartridgeStatus[NUMBER_OF_CARTRIDGES] = {ABSENT,
                                                                 ABSENT};

static bool cartridgeRemovalCheckEnabled = 1;

//===========================================================================
//====================== Private Functions Prototypes =======================
//===========================================================================

static void cartridgeAbsentUpdate(uint8_t cartNumber);
static void cartridgePresentUpdate(uint8_t cartNumber);
static bool cartridgesRemovedCheck(void);
static bool cartridgesPresentCheck(void);
static void _cartridge_removed_error(const char *serial_msg);

//===========================================================================
//============================ Public Functions =============================
//===========================================================================

/**
 * Checks to see if a cartridge is present, using the defined cartridge
 * addresses.
 * @inputs     0 for Cartridge 0, 1 for Cartridge 1
 * @returns    Returns true if the specified cartridge is present
 */
bool Cartridge__Present(uint8_t cartridgeNumber) {
  uint8_t returnValue = false;
  if (cartridgeStatus[cartridgeNumber] == PRESENT) returnValue = true;
  return returnValue;
}

/**
 * Checks to see if a cartridge is removed, using the defined cartridge
 * addresses.
 * @inputs     0 for Cartridge 0, 1 for Cartridge 1
 * @returns    Returns true if the specified cartridge is removed
 */
bool Cartridge__Removed(uint8_t cartridgeNumber) {
  uint8_t returnValue = false;
  if (cartridgeNumber <= NUMBER_OF_CARTRIDGES) {
    if (cartridgeStatus[cartridgeNumber] == REMOVED) returnValue = true;
  }
  return returnValue;
}

/**
* Check to see if cartridges are present or absent. Flags internally if
* one has been removed, or clears the removed flag if it's present.
* The status of cartridge removal can be found with
* CartridgeRemoved()
*/
void Cartridge__Update(void) {
  if (cartridgeRemovalCheckEnabled == true) {
    // Cartridge zero is pulled low by default
    if (READ(CART0_SIG2_PIN) == HIGH) {
      cartridgePresentUpdate(0);
    } else {
      cartridgeAbsentUpdate(0);
    }
    // Cartridge one is pulled high by default
    if (READ(CART1_SIG2_PIN) == LOW) {
      cartridgePresentUpdate(1);
    } else {
      cartridgeAbsentUpdate(1);
    }
  } else {
    cartridgePresentUpdate(0);
    cartridgePresentUpdate(1);
  }
}

/**
 * This function checks to see if the FFF cartridge is removed,
 * to prevent heating
 * @returns    Returns true if an FFF cartridge has been removed
 */
bool Cartridge__FFFNotPresent(void) {
  Cartridge__Update();
  return (cartridgeStatus[FFF_INDEX] != PRESENT);
}

/**
 * This function checks to see if the FFF cartridge is removed,
 * to prevent heating, with hysteresis for when the cartridge
 * is reinserted.
 * @returns    Returns true if an FFF cartridge has been removed
 */
bool Cartridge__FFFNotPresentHysteresis(void) {
  bool returnValue = false;
  static uint8_t cartridgeRemovalHysteresis = 0;

  Cartridge__Update();

  if (cartridgeStatus[FFF_INDEX] != PRESENT) {
    cartridgeRemovalHysteresis = CARTRIDGE_REMOVAL_HYSTERESIS_COUNT;
  }

  // We have this hysteresis here to accomodate putting the cartridge
  // back in. Without it, Marlin will recognize the cartridge has been
  // reinserted before the temperature updates from its maximum value and
  // will throw a high temperature error.
  if (cartridgeRemovalHysteresis > 0) {
    cartridgeRemovalHysteresis--;
    returnValue = true;
  }

  return returnValue;
}

/**
 * Allows all cartridge checks to be disabled by making the printer think
 * cartridges are always present.
 * @value     true = enable, false = no check.
 */
void Cartridge__SetPresentCheck(bool value) {
  if (value == true) {
    SERIAL_PROTOCOLLNPGM("Cartridge Check Enabled");
    cartridgeRemovalCheckEnabled = 1;
  } else if (value == false) {
    SERIAL_PROTOCOLLNPGM("Cartridge Check Disabled");
    cartridgeRemovalCheckEnabled = 0;
  } else {
    SERIAL_PROTOCOLLNPGM("Invalid value for Cartridge Check Set");
    return;
  }
  cartridgeRemovalCheckEnabled = value;
}

bool Cartridge__GetPresentCheck(void) { return cartridgeRemovalCheckEnabled; }

//===========================================================================
//============================ Private Functions ============================
//===========================================================================

/**
 * Reports that a cartridge is absent. If there was a cartridge present,
 * marks it as removed
 */
static void cartridgeAbsentUpdate(uint8_t cartNumber) {
  if (cartridgeStatus[cartNumber] == PRESENT) {
    cartridgeStatus[cartNumber] = REMOVED;
    switch (cartNumber) {
      case FFF_INDEX:
        SERIAL_PROTOCOLLNPGM("FFF Cartridge Removed");
        break;
      case SILVER_INDEX:
        // Prevents the silver extruder from being lowered
        // unintentionally
        WRITE(CART1_SIG1_PIN, LOW);
        SERIAL_PROTOCOLLNPGM("Silver Cartridge Removed");
        break;
      default:
        SERIAL_PROTOCOLLNPGM("Cartridge Removed");
    }
    _cartridge_removed_error((PSTR(MSG_T_CARTRIDGE_REMOVED)));
  }
  cartridgeStatus[cartNumber] = ABSENT;
}

/**
 * Reports that a cartridge is present. If it was marked as removed,
 * this will clear it.
 */
static void cartridgePresentUpdate(uint8_t cartNumber) {
  if (cartridgeStatus[cartNumber] != PRESENT) {
    switch (cartNumber) {
      case FFF_INDEX:
        SERIAL_PROTOCOLLNPGM("FFF Cartridge Inserted");
        break;
      case SILVER_INDEX:
        SERIAL_PROTOCOLLNPGM("Silver Cartridge Inserted");
        break;
      default:
        SERIAL_PROTOCOLLNPGM("Cartridge Inserted");
    }
  }
  cartridgeStatus[cartNumber] = PRESENT;
}

/**
 * Signals that no cartridges have been removed, used to tell if the system
 * is safe to run after a reset with cartridges removed.
 * @returns    Returns true if a catridge has been removed, false otherwise.
 */
static bool cartridgesRemovedCheck(void) {
  for (uint8_t i = 0; i < NUMBER_OF_CARTRIDGES; i++) {
    if (cartridgeStatus[i] == REMOVED) {
      return true;
    }
  }
  return false;
}

/**
 * Signals that no cartridges are present, used to tell if the system
 * is safe to run after a reset with cartridges removed.
 * @returns    Returns true if a catridge is present, false otherwise.
 */
static bool cartridgesPresentCheck(void) {
  for (uint8_t i = 0; i < NUMBER_OF_CARTRIDGES; i++) {
    if (cartridgeStatus[i] == PRESENT) {
      return true;
    }
  }
  return false;
}

/**
 * This is the error handler for when we see the cartridge removed error. It
 * reports to octoprint that we should pause the print, as well as disabling
 * all heaters and stopping the currently queued commands. This will only get
 * called once if it's been called multiple times in quick succession.
 * @inputs     An input that will be displayed on the serial monitor
 */
static void _cartridge_removed_error(const char *serial_msg) {
  if (IsSafetyCriticalSection()) {
    static bool killed = false;
    if (IsRunning()) {
      SERIAL_ERROR_START;
      serialprintPGM(serial_msg);
      SERIAL_EOL;
    }
    if (!killed) {
      Running = false;
      killed = true;
      kill(serial_msg);
    }
  } else {
    quickStop();
    disable_all_heaters();
    disable_all_steppers();
    serialprintPGM(serial_msg);
    SERIAL_EOL;
    SERIAL_PROTOCOLLNPGM("// action:cancel");
  }
}
