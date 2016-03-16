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
//============================= Public Functions ============================
//===========================================================================

/**
 * This function checks to see if the FFF cartridge is removed,
 * to prevent heating
 * @returns    Returns true if an FFF cartridge has been removed
 */
  bool CartridgeRemovedFFF(void);

/**
 * This function checks to see if a cartridge has been removed from the
 * system, allowing us to make judgement calls for error reporting. This 
 * also updates the cartridge status.
 * @returns    Returns true if a cartridge has been removed 
 */
  bool CartridgeRemoved(void);

/**
 * This is the error handler for when we see the cartridge removed error. It 
 * reports to octoprint that we should pause the print, as well as disabling
 * all heaters and stopping the currently queued commands. This will only get
 * called once if it's been called multiple times in quick succession.
 */
  void _cartridge_removed_error(const char *serial_msg);

#endif  // MARLIN_CARTRIDGE_H_
