/**
 * Cartridge.h - Used to keep track of the status of the cartridges.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 2/19/2016
 * Used to keep track of all information about material cartridges, for use
 * in decision making in other parts of the program
 * Copyright (C) 2016 Voxel8
 */

#ifndef MARLIN_CARTRIDGE_H_
#define MARLIN_CARTRIDGE_H_

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

#define NUMBER_OF_CARTRIDGES (2)
#define CARTRIDGE_REMOVAL_HYSTERESIS_COUNT (250)
#define FFF_INDEX (0)
#define SILVER_INDEX (1)

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

/**
 * Checks to see if a cartridge is present, using the defined cartridge
 * addresses.
 * @returns    Returns true if the specified cartridge is present
 */
  bool Cartridge__Present(uint8_t cartridgeNumber);

/**
 * This function checks to see if the FFF cartridge is removed,
 * to prevent heating
 * @returns    Returns true if an FFF cartridge has been removed
 */
  bool Cartridge__FFFNotPresent(void);

 /**
 * This function checks to see if the FFF cartridge is removed,
 * to prevent heating, with hysteresis for when the cartridge 
 * is reinserted.
 * @returns    Returns true if an FFF cartridge has been removed
 */
  bool Cartridge__FFFNotPresentHysteresis(void);

 /**
 * Check to see if cartridges are present or absent. Flags internally if 
 * one has been removed, or clears the removed flag if it's present. 
 * The status of cartridge removal can be found with 
 * CartridgeRemoved()
 */
  void Cartridge__Update(void);

 /** 
  * Allows all cartridge checks to be disabled by making the printer think
  * cartridges are always present.
  * @value     true = enable, false = no check.
  */
  void Cartridge__SetPresentCheck(bool value);

 /** 
  * Checks to see if the cartridge present check is active
  * @returns   Returns true if the check is active
  */
  bool Cartridge__GetPresentCheck(void);

 /**
  * Enables or disables auger extrusion
  * @value     true = enable, false = disable
  */
  void Cartridge__SetAugerEnabled(bool value);

 /**
  * Checks to see if auger extrusion is enabled
  * @returns   Returns true if auger extrusion is enabled
  */
  bool Cartridge__GetAugerEnabled(void);
#endif  // MARLIN_CARTRIDGE_H_
