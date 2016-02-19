/**
 * Cartridge.cpp - Used to keep track of the status of the cartridges.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 2/19/2016
 * Used to keep track of all information about material cartridges, for use
 * in decision making in other parts of the program
 */

#include "Marlin.h"
#include "Cartridge.h"

#define NUMBER_OF_CARTRIDGES   (2)


//===========================================================================
//============================= Private Variables ===========================
//===========================================================================

static bool cartridgePresent[NUMBER_OF_CARTRIDGES] = {false,false};
static bool cartridgeRemoved[NUMBER_OF_CARTRIDGES]  = {false,false};

//===========================================================================
//======================= Private Functions Prototypes ======================
//===========================================================================

static void cartridgeAbsentUpdate(unsigned int cartNumber);
static void cartridgePresentUpdate(unsigned int cartNumber);

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

 /**
 * Check to see if cartridges are present or absent. Flags internally if 
 * one has been removed, or clears the removed flag if it's present. 
 * The status of cartridge removal can be found with CartridgeRemoved()
 */
void UpdateCartridgeStatus(void)
{
	// Cartridge zero is pulled low by default
	if(READ(CART0_SIG2_PIN) == HIGH)
	{
		cartridgePresentUpdate(0);
	}
	else
	{
		cartridgeAbsentUpdate(0);
	}
	// Cartridge one is pulled high by default
	if(READ(CART1_SIG2_PIN) == LOW)
	{
		cartridgePresentUpdate(1);
	}
	else
	{
		cartridgeAbsentUpdate(1);
	}
}  

/**
 * This function checks to see if a cartridge has been removed from the
 * system, allowing us to make judgement calls for error reporting. This
 * information is updated by calling UpdateCartridgeStatus. Will also 
 * return true if no cartridges are present.
 * @returns    Returns true if a cartridge has been removed, or no 
 *             cartridge is present 
 */
bool CartridgeRemoved(void)
{
	bool returnValue = false;
	unsigned int cartridgePresentSum = 0;
	for (unsigned int i= 0; i < NUMBER_OF_CARTRIDGES; i++)
	{
		if (cartridgeRemoved[i] == true)
		{
			returnValue = true;
			cartridgePresentSum += cartridgePresent[i];
		}
	}
	if (cartridgePresentSum == 0)
	{
		returnValue = true;
	}
	return returnValue;
}

//===========================================================================
//============================ Private Functions ============================
//===========================================================================

/**
 * Reports that a cartridge is absent. If there was a cartridge present,
 * marks it as removed
 */
static void cartridgeAbsentUpdate(unsigned int cartNumber)
{
	if (cartridgePresent[cartNumber] == true)
	{
		cartridgeRemoved[cartNumber] = true;
	}
	cartridgePresent[cartNumber] = false;
}

/**
 * Reports that a cartridge is present. If it was marked as removed,
 * this will clear it.
 */
static void cartridgePresentUpdate(unsigned int cartNumber)
{
	cartridgePresent[cartNumber] = true;
	cartridgeRemoved[cartNumber] = false;
}