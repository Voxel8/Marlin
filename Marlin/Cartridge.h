/**
 * Cartridge.h - Used to keep track of the status of the cartridges.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 2/19/2016
 * Used to keep track of all information about material cartridges, for use
 * in decision making in other parts of the program
 */

#ifndef cartridge_h
#define cartridge_h 

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

 /**
 * Check to see if cartridges are present or absent. Flags internally if 
 * one has been removed, or clears the removed flag if it's present. 
 * The status of cartridge removal can be found with CartridgeRemoved
 */
 void UpdateCartridgeStatus(void);

/**
 * This function checks to see if a cartridge has been removed from the
 * system, allowing us to make judgement calls for error reporting. This
 * information is updated by calling UpdateCartridgeStatus
 * @returns    Returns true if a cartridge has been removed 
 */
 bool CartridgeRemoved(void);

/**
 * This function checks to see if a cartridge has been removed from the
 * system, allowing us to make judgement calls for error reporting. This
 * information is updated by calling UpdateCartridgeStatus
 * @returns    Returns true if a cartridge has been removed 
 */
 bool CartridgeRemovedQuickResponse(void);
#endif