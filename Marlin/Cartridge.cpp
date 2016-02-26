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

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

#define NUMBER_OF_CARTRIDGES               (2)
#define CARTRIDGE_REMOVAL_HYSTERESIS_COUNT (4)

//===========================================================================
//============================ Private Variables ============================
//===========================================================================

static bool cartridgePresent[NUMBER_OF_CARTRIDGES] = {false,false};
static bool cartridgeRemoved[NUMBER_OF_CARTRIDGES] = {false,false};
static bool cartridgeRemovedSafeToMove             = false;

//===========================================================================
//====================== Private Functions Prototypes =======================
//===========================================================================

static void cartridgeAbsentUpdate(unsigned int cartNumber);
static void cartridgePresentUpdate(unsigned int cartNumber);
static bool cartridgesRemovedCheck(void);
static bool cartridgesPresentCheck(void);

//===========================================================================
//============================ Public Functions =============================
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
 * information is updated by calling UpdateCartridgeStatus(). Will also 
 * return true if no cartridges are present.
 * @returns    Returns true if a cartridge has been removed, or no 
 *             cartridge is present 
 */
bool CartridgeRemoved(void)
{
    bool returnValue = false;
    static unsigned int cartridgeRemovalHysteresis = 0;
    bool removedCondition = (!cartridgePresent[0] || !cartridgesPresentCheck());

    // If a cartridge is seen to be removed, set the hysteresis counter.
    if(cartridgesRemovedCheck())
    {
        cartridgeRemovalHysteresis = CARTRIDGE_REMOVAL_HYSTERESIS_COUNT;
    }

    // If the appropriate cartridges are absent but we don't see that a cartridge
    // has been removed, we started up with it missing and will be able to operate
    // but not activate the hot ends. 
    else if (removedCondition && !cartridgesRemovedCheck())
    {
        cartridgeRemovalHysteresis = CARTRIDGE_REMOVAL_HYSTERESIS_COUNT;
    }

    // We have this hysteresis here to accomodate putting the cartridge 
    // back in. Without it, Marlin will recognize the cartridge has been
    // reinserted before the temperature updates from its maximum value and
    // will throw a high temperature error.
    if (cartridgeRemovalHysteresis > 0)
    {
        cartridgeRemovalHysteresis--;
        returnValue = true;
    }

    return returnValue;
}

bool CartridgeRemovedQuickResponse(void)
{
    bool returnValue = false;
    bool removedCondition = (!cartridgePresent[0] || !cartridgesPresentCheck());
    // If a cartridge is seen to be removed, set the hysteresis counter.
    if(cartridgesRemovedCheck())
    {
        returnValue = true;
    }

    // If the appropriate cartridges are absent but we don't see that a cartridge
    // has been removed, we started up with it missing and will be able to operate
    // but not activate the hot ends. 
    else if (removedCondition && !cartridgesRemovedCheck())
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
        if (cartNumber == 1)
        {
            WRITE(CART1_SIG1_PIN, LOW); // Prevents the silver extruder from
                                        // being lowered unintentionally
        }
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

/**
 * Signals that no cartridges have been removed, used to tell if the system 
 * is safe to run after a reset with cartridges removed.
 * @returns    Returns true if no catridges have been removed, false otherwise.
 */
static bool cartridgesRemovedCheck(void)
{
    for (unsigned int i= 0; i < NUMBER_OF_CARTRIDGES; i++)
    {
        if (cartridgeRemoved[i])
        {
            return true;
        }
    }
    return false;
}

/**
 * Signals that no cartridges are present, used to tell if the system 
 * is safe to run after a reset with cartridges removed.
 * @returns    Returns true if no catridges are present, false otherwise.
 */
static bool cartridgesPresentCheck(void)
{
    for (unsigned int i= 0; i < NUMBER_OF_CARTRIDGES; i++)
    {
        if (cartridgePresent[i])
        {
            return true;
        }
    }
    return false;
}
