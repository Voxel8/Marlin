/***********************************************************************************
* For use with the Texas Instruments ADS1015 16-bit ADC
*
* @author Ricky Rininger 02/03/2015
*
***********************************************************************************/

#ifndef ADS1015_H
#define ADS1015_H

#include "Arduino.h"
#include <Wire.h>

/* I2C ADDRESS */
/*-------------*/
#define ADS1015_I2C_ADDRESS 0x48 // CHANGE THIS IF YOUR ADDRESS IS NOT 0x48 !!!!!

/* CONVERSION DELAY (ms) */
/*-----------------------*/
#define ADS1015_CONVERSION_DELAY 8     // Delay to allow conversion to finish

/* POINTER REGISTER DEFINITIONS */
/*------------------------------*/
#define ADS1015_CONVERSION_REG  0X00 // Read Only
#define ADS1015_CONFIG_REG      0x01
#define ADS1015_LO_THRESH_REG   0x02
#define ADS1015_HI_THRESH_REG   0x03

/* CONFIG REGISTER DEFINITIONS */
/*-----------------------------*/
#define ADS1015_OS_SINGLE       0x8000 // Start conversion (WRITE)
#define ADS1015_OS_BUSY         0x0000 // Bit = 0 if conversion in progress (READ)
#define ADS1015_OS_NOTBUSY      0x8000 // Bit = 1 if no conversion in progress (READ)

#define ADS1015_MUX_DIFF_0_1    0x0000 // Differential P = 0, N = 1  
#define ADS1015_MUX_DIFF_0_3    0x1000 // Differential P = 0, N = 3
#define ADS1015_MUX_DIFF_1_3    0x2000 // Differential P = 1, N = 3
#define ADS1015_MUX_DIFF_2_3    0x3000 // Differential P = 2, N = 3
#define ADS1015_MUX_SINGLE_00   0x4000 // Channel 0 single-ended operation
#define ADS1015_MUX_SINGLE_01   0x5000 // Channel 1 single-ended operation
#define ADS1015_MUX_SINGLE_02   0x6000 // Channel 2 single-ended operation
#define ADS1015_MUX_SINGLE_03   0x7000 // Channel 3 single-ended operation

#define ADS1015_PGA_6144        0x0000 // PGA Gain = 2/3
#define ADS1015_PGA_4096        0x0200 // PGA Gain = 1
#define ADS1015_PGA_2048        0x0400 // PGA Gain = 2 (DEFAULT)
#define ADS1015_PGA_1024        0x0600 // PGA Gain = 4
#define ADS1015_PGA_0512        0x0800 // PGA Gain = 8
#define ADS1015_PGA_0256        0x0A00 // PGA Gain = 16

#define ADS1015_MODE_CONT       0x0000 // Continuous conversion mode
#define ADS1015_MODE_SINGLE     0x0100 // Single-shot mode (DEFAULT)

#define ADS1015_DR_128          0x0000 // Data Rate = 128 SPS
#define ADS1015_DR_250          0x0020 // Data Rate = 250 SPS
#define ADS1015_DR_490          0x0040 // Data Rate = 490 SPS
#define ADS1015_DR_920          0x0060 // Data Rate = 920 SPS
#define ADS1015_DR_1600         0x0080 // Data Rate = 1600 SPS (DEFAULT)
#define ADS1015_DR_2400         0x00A0 // Data Rate = 2400 SPS
#define ADS1015_DR_3300         0x00C0 // Data Rate = 3300 SPS
// 0x00E0 is also 3300 SPS

#define ADS1015_COMP_MODE_TRAD  0x0000 // Traditional Comparator w/ hysteresis (DEFAULT)
#define ADS1015_COMP_MODE_WIND  0x0010 // Window Comparator

#define ADS1015_COMP_POL_ACTLO  0x0000 // Comparator output active low (DEFAULT)
#define ADS1015_COMP_POL_ACTHI  0x0008 // Comparator output active high

#define ADS1015_COMP_NONLATCH   0x0000 // Non-latching comparator (DEFAULT)
#define ADS1015_COMP_LATCHING   0x0004 // Latching comparator

#define ADS1015_COMP_QUE_1CONV  0x0000 // Assert after one conversion
#define ADS1015_COMP_QUE_2CONV  0x0001 // Assert after two conversions
#define ADS1015_COMP_QUE_4CONV  0x0002 // Assert after four conversions
#define ADS1015_COMP_DISABLE    0x0003 // Disable comparator (DEFAULT)
/*--------------------------------------------------------------------------------*/
    
/* Definition for M125 */
/*---------------------*/
#define EXT_ADC_READ readADC_SingleEnded(0) // Reads channel 0 in single-ended mode

/* Definitions for calculations */
/*------------------------------*/

// TODO: set this to auto-configure conversion factor based on a value set in the config.h file
#define CONV_FACTOR     0.003001    // (6.144V / 2047 bits) 
#define VOLT_TO_DIST    2000        // (10000 um / 5V)



/* Function Prototypes */
/*=====================*/

uint16_t readADC_SingleEnded(uint8_t channel); // tested and working well

uint16_t readADC_Differential(uint8_t first_channel, uint8_t second_channel);

void regWrite(uint8_t address, uint8_t reg, uint16_t value);

uint16_t regRead(uint8_t address, uint8_t reg);

#endif