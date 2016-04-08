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

bool  code_seen(char code);
float code_value(void);

//===========================================================================
//============================= Public Variables ============================
//===========================================================================

extern const char axis_codes[NUM_AXIS];

#endif  // G_CODE_UTILITY_H_
