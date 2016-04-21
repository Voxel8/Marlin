/**
 * GCodeUtility.h - Interface for useful functions from Marlin_main.cpp
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/8/2016
 * Useful functions from Marlin_main.cpp, so that they can be used elsewhere.
 * Copyright (C) 2016 Voxel8
 */

#ifndef G_CODE_UTILITY_H_
#define G_CODE_UTILITY_H_

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

// Returns if a specific identifier letter was used in an MCode. 
bool  code_seen(char code);

// Returns the value of the last item identified by code_seen.
float code_value(void);

//===========================================================================
//============================= Public Variables ============================
//===========================================================================

extern const char axis_codes[NUM_AXIS];

#endif  // G_CODE_UTILITY_H_
