/**
 * HeatedBed.h - Used to keep track of the status of the heated bed.
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 3/24/2016
 * Used to keep track of all information about the headed bed, for use
 * in decision making in other parts of the program
 * Copyright (C) 2016 Voxel8
 */

#ifndef MARLIN_HEATED_BED_H_
#define MARLIN_HEATED_BED_H_

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

/**
 * Checks to see if the heated bed is present, and reports if it's not
 * @returns    Returns true if the heated bed is present
 */
bool HeatedBed__PresentCheck(void);
void HeatedBed__SetPresentCheck(bool value);

#endif  // MARLIN_HEATED_BED_H_
