/**
 * GCodes.h - Mcode Interface for GCodes.cpp
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/8/2016
 * Used as an interface for Gcodes, to decouple them from Marlin_main.
 * Copyright (C) 2016 Voxel8
 */

#ifndef G_CODES_H_
#define G_CODES_H_

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

void gcode_M272(void);

#endif  // G_CODES_H_