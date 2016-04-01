/**
 * HeatedBed.cpp - Used to keep track of the status of the heated bed.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 3/24/2016
 * Used to keep track of all information about the headed bed, for use
 * in decision making in other parts of the program
 * Copyright (C) 2016 Voxel8
 */

#include "Marlin.h"
#include "HeatedBed.h"
#include "stepper.h"     // for quickstop()
#include "temperature.h" // for disable_all_heaters()
 
//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

//===========================================================================
//============================ Private Variables ============================
//===========================================================================

//===========================================================================
//====================== Private Functions Prototypes =======================
//===========================================================================

void heatedBedRemovedError(void);

//===========================================================================
//============================ Public Functions =============================
//===========================================================================

/**
 * Checks to see if the heated bed is present, and reports if it's not
 * @returns    Returns true if the heated bed is present
 */
bool HeatedBed__PresentCheck(void) {
	#if ENABLED(HEATED_BED_PRESENT_CHECK)
		bool returnValue = false;
		if (READ(BED_AVAIL_PIN) == HIGH) {
			returnValue = true;
		}
		else {
			heatedBedRemovedError();
		}
		return returnValue;
	#else
		return true;
	#endif
}

//===========================================================================
//============================ Private Functions ============================
//===========================================================================

void heatedBedRemovedError(void) {
    static millis_t timeSinceLastRemoval = {0};
    if (millis() > timeSinceLastRemoval + 1000) {
        quickStop();
        disable_all_heaters();
        disable_all_steppers();
        SERIAL_PROTOCOL("Heated Bed Removed!");
        SERIAL_EOL;
        SERIAL_ECHOLN("// action:pause");
    }
    timeSinceLastRemoval = millis();
}