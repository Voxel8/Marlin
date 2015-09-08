/***********************************************************************************
* This file contains the conditional includes that depend on the ADC definition
* in Configuration.h
*
* @author Ricky Rininger
***********************************************************************************/

#ifndef EXT_ADC_H
#define	EXT_ADC_H

#if EXT_ADC == 1
	#include "ADS1115.h"

#elif EXT_ADC == 2
	#include "ADS1015.h"
#endif

#define ADC_SAMPLE_POWER    0x0F   // (15 in decimal) Max pow of 2 that can be given as arg 
                                       // for M234/5.

#endif // EXT_ADC.h
