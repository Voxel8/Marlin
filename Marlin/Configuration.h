#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "boards.h"
#include "macros.h"


#define USE_AUTOMATIC_VERSIONING 1

#if ENABLED(USE_AUTOMATIC_VERSIONING)
  #include "_Version.h"
#else
  #include "Default_Version.h"
#endif

// User-specified version info of this build to display in [Pronterface, etc] terminal window during
// startup. Implementation of an idea by Prof Braino to inform user that any changes made to this
// build by the user have been successfully uploaded into firmware.
#define STRING_CONFIG_H_AUTHOR "Voxel8" // Who made the changes.
#define SHOW_BOOTSCREEN
#define STRING_SPLASH_LINE1 SHORT_BUILD_VERSION // will be shown during bootup in line 1
//#define STRING_SPLASH_LINE2 STRING_DISTRIBUTION_DATE // will be shown during bootup in line 2

// @section machine

// SERIAL_PORT selects which serial port should be used for communication with the host.
// This allows the connection of wireless adapters (for instance) to non-default port pins.
// Serial port 0 is still used by the Arduino bootloader regardless of this setting.
// :[0,1,2,3,4,5,6,7]
#define SERIAL_PORT 0

// This determines the communication speed of the printer
// :[2400,9600,19200,38400,57600,115200,250000]
#define BAUDRATE 250000

// The following define selects which electronics board you have.
// Please choose the name from boards.h that matches your setup
#ifndef MOTHERBOARD
  #define MOTHERBOARD BOARD_VOXEL8_GEN3C2
#endif

// Define this to set a unique identifier for this printer, (Used by some programs to differentiate between machines)
// You can use an online service to generate a random UUID. (eg http://www.uuidgenerator.net/version4)
//#define MACHINE_UUID "00000000-0000-0000-0000-000000000000"

// This defines the number of extruders
// :[1,2,3,4]
#define EXTRUDERS 3

// Offset of the extruders (uncomment if using more than one and relying on firmware to position when changing).
// The offset has to be X=0, Y=0 for the extruder 0 hotend (default extruder).
// For the other hotends it is their distance from the extruder 0 hotend.
//#define EXTRUDER_OFFSET_X {0.0, 20.00} // (in mm) for each extruder, offset of the hotend on the X axis
//#define EXTRUDER_OFFSET_Y {0.0, 5.00}  // (in mm) for each extruder, offset of the hotend on the Y axis

//// The following define selects which power supply you have. Please choose the one that matches your setup
// 1 = ATX
// 2 = X-Box 360 203Watts (the blue wire connected to PS_ON and the red wire to VCC)
// :{1:'ATX',2:'X-Box 360'}

#define POWER_SUPPLY 1


//===========================================================================
//============================== External ADC ===============================
//===========================================================================
// This feature allows for the use of an external ADC *in addition* to the on-board 10-bit ADC
//
//// Select the appropriate ADC according to the number associated with the chip you are using
// 1 = Texas Instruments ADS1115 (16-bit)
// 2 = Texas Instruments ADS1015 (12-bit)
//
// Define this to allow the use of an external ADC 
#define EXT_ADC 1

// Define ADC mode of operation (Single-ended or Differential) for the TI chips
// ** This definition ONLY affects code that refers to EXT_ADC_RAW_x.
// ** Even if a mode is defined here, both single-ended and differential readings
// ** can be taken by using the functions defind in ADS1x15.cpp 

// 1 = Single-Ended Mode
// 2 = Differential Mode
#if EXT_ADC == (1 || 2)
  #define EXT_ADC_MODE 2
#endif


//===========================================================================
//============================= Mechanical Settings =========================
//===========================================================================

// @section machine

// Uncomment this option to enable CoreXY kinematics
#define COREXY

// Uncomment this option to enable CoreXZ kinematics
//#define COREXZ

// Enable this option for Toshiba steppers
//#define CONFIG_STEPPERS_TOSHIBA

// @section homing

// coarse Endstop Settings
#define ENDSTOPPULLUPS // Comment this out (using // at the start of the line) to disable the endstop pullup resistors

#if DISABLED(ENDSTOPPULLUPS)
  // fine endstop settings: Individual pullups. will be ignored if ENDSTOPPULLUPS is defined
  //#define ENDSTOPPULLUP_XMAX
  //#define ENDSTOPPULLUP_YMAX
  //#define ENDSTOPPULLUP_ZMAX
  //#define ENDSTOPPULLUP_XMIN
  //#define ENDSTOPPULLUP_YMIN
  //#define ENDSTOPPULLUP_ZMIN
  //#define ENDSTOPPULLUP_ZMIN_PROBE
#endif

// Mechanical endstop with COM to ground and NC to Signal uses "false" here (most common setup).
const bool X_MIN_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
const bool Y_MIN_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
const bool Z_MIN_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
const bool X_MAX_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
const bool Y_MAX_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
const bool Z_MAX_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
const bool Z_MIN_PROBE_ENDSTOP_INVERTING = true; // set to true to invert the logic of the endstop.
//#define DISABLE_MAX_ENDSTOPS
//#define DISABLE_MIN_ENDSTOPS

// If you want to enable the Z probe pin, but disable its use, uncomment the line below.
// This only affects a Z probe endstop if you have separate Z min endstop as well and have
// activated Z_MIN_PROBE_ENDSTOP below. If you are using the Z Min endstop on your Z probe,
// this has no effect.
//#define DISABLE_Z_MIN_PROBE_ENDSTOP

// For Inverting Stepper Enable Pins (Active Low) use 0, Non Inverting (Active High) use 1
// :{0:'Low',1:'High'}
#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0 // For all extruders

// Disables axis when it's not being used.
// WARNING: When motors turn off there is a chance of losing position accuracy!
#define DISABLE_X false
#define DISABLE_Y false
#define DISABLE_Z false

// @section extruder

#define DISABLE_E false // For all extruders
#define DISABLE_INACTIVE_EXTRUDER true //disable only inactive extruders and keep active extruder enabled

// @section machine

// Invert the stepper direction. Change (or reverse the motor connector) if an axis goes the wrong way.
#define INVERT_X_DIR false
#define INVERT_Y_DIR false
#define INVERT_Z_DIR false

// @section extruder

// For direct drive extruder v9 set to true, for geared extruder set to false.
#define INVERT_E0_DIR false
#define INVERT_E1_DIR false
#define INVERT_E2_DIR false
#define INVERT_E3_DIR false

// @section homing

// ENDSTOP SETTINGS:
// Sets direction of endstops when homing; 1=MAX, -1=MIN
// :[-1,1]
#define X_HOME_DIR -1
#define Y_HOME_DIR 1
#define Z_HOME_DIR -1

#define min_software_endstops true // If true, axis won't move to coordinates less than HOME_POS.
#define max_software_endstops true  // If true, axis won't move to coordinates greater than the defined lengths below.

// @section machine

// Travel limits after homing (units are in mm)
#define X_MIN_POS -38
#define Y_MIN_POS 0
#define Z_MIN_POS 0
#define X_MAX_POS 150
#define Y_MAX_POS 188
#define Z_MAX_POS 105
#define X_BED_MAX 150
#define Y_BED_MAX 150
#define X_BED_CENTER (X_BED_MAX / 2)
#define Y_BED_CENTER (Y_BED_MAX / 2) 
//===========================================================================
//========================= Filament Runout Sensor ==========================
//===========================================================================
//#define FILAMENT_SENSOR   // Enables use of optical filament sensor
//#define FILAMENT_RUNOUT_SENSOR // Uncomment for defining a filament runout sensor such as a mechanical or opto endstop to check the existence of filament
                                 // In RAMPS uses servo pin 2. Can be changed in pins file. For other boards pin definition should be made.
                                 // It is assumed that when logic high = filament available
                                 //                    when logic  low = filament ran out
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  const bool FIL_RUNOUT_INVERTING = true;  // Should be uncommented and true or false should assigned
  #define ENDSTOPPULLUP_FIL_RUNOUT // Uncomment to use internal pullup for filament runout pins if the sensor is defined.
  #define FILAMENT_RUNOUT_SCRIPT "M600"
#endif


//===========================================================================
//============================ Bed Auto Leveling ============================
//===========================================================================

// @section bedlevel

#define AUTO_BED_LEVELING_FEATURE // Delete the comment to enable (remove // at the start of the line)
//#define DEBUG_LEVELING_FEATURE
#define Z_MIN_PROBE_REPEATABILITY_TEST  // If not commented out, Z-Probe Repeatability test will be included if Auto Bed Leveling is Enabled.

#if ENABLED(AUTO_BED_LEVELING_FEATURE)

  // There are 2 different ways to specify probing locations:
  //
  // - "grid" mode
  //   Probe several points in a rectangular grid.
  //   You specify the rectangle and the density of sample points.
  //   This mode is preferred because there are more measurements.
  //
  // - "3-point" mode
  //   Probe 3 arbitrary points on the bed (that aren't colinear)
  //   You specify the XY coordinates of all 3 points.

  // Enable this to sample the bed in a grid (least squares solution).
  // Note: this feature generates 10KB extra code size.
  //#define AUTO_BED_LEVELING_GRID

  // Offsets to the probe relative to the extruder tip (Hotend - Probe)
  // X and Y offsets must be integers
  #define X_PROBE_OFFSET_FROM_EXTRUDER 18     // Probe on: -left  +right
  #define Y_PROBE_OFFSET_FROM_EXTRUDER -46    // Probe on: -front +behind
  #define Z_PROBE_OFFSET_FROM_EXTRUDER -3     // -below (always!)

  #if ENABLED(AUTO_BED_LEVELING_GRID)

    #define LEFT_PROBE_BED_POSITION 15
    #define RIGHT_PROBE_BED_POSITION 170
    #define FRONT_PROBE_BED_POSITION 20
    #define BACK_PROBE_BED_POSITION 170

    #define MIN_PROBE_EDGE 10 // The Z probe minimum square sides can be no smaller than this.

    // Set the number of grid points per dimension.
    // You probably don't need more than 3 (squared=9).
    #define AUTO_BED_LEVELING_GRID_POINTS 2

  #else  // !AUTO_BED_LEVELING_GRID

      // Arbitrary points to probe. A simple cross-product
      // is used to estimate the plane of the bed.
      #define ABL_PROBE_PT_1_X X_MIN_POS + 25 + X_PROBE_OFFSET_FROM_EXTRUDER
      #define ABL_PROBE_PT_1_Y 60 + Y_PROBE_OFFSET_FROM_EXTRUDER
      #define ABL_PROBE_PT_2_X X_MAX_POS/2
      #define ABL_PROBE_PT_2_Y 175 + Y_PROBE_OFFSET_FROM_EXTRUDER   // you can't use x_max or y_max as variables because we made the bed size larger than it actually is
      #define ABL_PROBE_PT_3_X X_MAX_POS - 25 + X_PROBE_OFFSET_FROM_EXTRUDER
      #define ABL_PROBE_PT_3_Y 60 + Y_PROBE_OFFSET_FROM_EXTRUDER

  #endif // AUTO_BED_LEVELING_GRID

  #define Z_RAISE_BEFORE_HOMING 10    // (in mm) Raise Z before homing (G28) for Probe Clearance.

  #define XY_TRAVEL_SPEED 8000        // X and Y axis travel speed between probes, in mm/min.

  #define Z_RAISE_BEFORE_PROBING 5    //How much the extruder will be raised before traveling to the first probing point.
  #define Z_RAISE_BETWEEN_PROBINGS 5  //How much the extruder will be raised when traveling from between next probing points
  #define Z_RAISE_AFTER_PROBING 5     //How much the extruder will be raised after the last probing point.

//#define Z_PROBE_END_SCRIPT "G1 Z10 F12000\nG1 X15 Y330\nG1 Z0.5\nG1 Z10" // These commands will be executed in the end of G29 routine.
                                                                            // Useful to retract a deployable Z probe.



  //If you have enabled the Bed Auto Leveling and are using the same Z Probe for Z Homing,
  //it is highly recommended you let this Z_SAFE_HOMING enabled!!!

  #define Z_SAFE_HOMING   // This feature is meant to avoid Z homing with Z probe outside the bed area.
                          // When defined, it will:
                          // - Allow Z homing only after X and Y homing AND stepper drivers still enabled.
                          // - If stepper drivers timeout, it will need X and Y homing again before Z homing.
                          // - Position the Z probe in a defined XY point before Z Homing when homing all axis (G28).
                          // - Block Z homing only when the Z probe is outside bed area.

  #if ENABLED(Z_SAFE_HOMING)
    #if ENABLED(HOME_AT_BACK)
      #define Z_SAFE_HOMING_X_POINT (100 + X_MIN_POS + X_PROBE_OFFSET_FROM_EXTRUDER)    // X point for Z homing when homing all axis (G28)
      #define Z_SAFE_HOMING_Y_POINT (150 + Y_MIN_POS + Y_PROBE_OFFSET_FROM_EXTRUDER)    // Y point for Z homing when homing all axis (G28)
    #else
      #define Z_SAFE_HOMING_X_POINT (X_BED_CENTER)    // X point for Z homing when homing all axis (G28)
      #define Z_SAFE_HOMING_Y_POINT (Y_BED_CENTER)    // Y point for Z homing when homing all axis (G28)
    #endif
  #endif

  // Support for a dedicated Z probe endstop separate from the Z min endstop.
  // If you would like to use both a Z probe and a Z min endstop together,
  // uncomment #define Z_MIN_PROBE_ENDSTOP and read the instructions below.
  // If you still want to use the Z min endstop for homing, disable Z_SAFE_HOMING above.
  // Example: To park the head outside the bed area when homing with G28.
  //
  // WARNING:
  // The Z min endstop will need to set properly as it would without a Z probe
  // to prevent head crashes and premature stopping during a print.
  //
  // To use a separate Z probe endstop, you must have a Z_MIN_PROBE_PIN
  // defined in the pins_XXXXX.h file for your control board.
  // If you are using a servo based Z probe, you will need to enable NUM_SERVOS,
  // Z_ENDSTOP_SERVO_NR and SERVO_ENDSTOP_ANGLES in the R/C SERVO support below.
  // RAMPS 1.3/1.4 boards may be able to use the 5V, Ground and the D32 pin
  // in the Aux 4 section of the RAMPS board. Use 5V for powered sensors,
  // otherwise connect to ground and D32 for normally closed configuration
  // and 5V and D32 for normally open configurations.
  // Normally closed configuration is advised and assumed.
  // The D32 pin in Aux 4 on RAMPS maps to the Arduino D32 pin.
  // Z_MIN_PROBE_PIN is setting the pin to use on the Arduino.
  // Since the D32 pin on the RAMPS maps to D32 on Arduino, this works.
  // D32 is currently selected in the RAMPS 1.3/1.4 pin file.
  // All other boards will need changes to the respective pins_XXXXX.h file.
  //
  // WARNING:
  // Setting the wrong pin may have unexpected and potentially disastrous outcomes.
  // Use with caution and do your homework.
  //
  //#define Z_MIN_PROBE_ENDSTOP

#endif // AUTO_BED_LEVELING_FEATURE


// @section homing

// The position of the homing switches
//#define MANUAL_HOME_POSITIONS  // If defined, MANUAL_*_HOME_POS below will be used
//#define BED_CENTER_AT_0_0  // If defined, the center of the bed is at (X=0, Y=0)

// Manual homing switch locations:
// For deltabots this means top and center of the Cartesian print volume.
#if ENABLED(MANUAL_HOME_POSITIONS)
  #define MANUAL_X_HOME_POS 0
  #define MANUAL_Y_HOME_POS 0
  #define MANUAL_Z_HOME_POS 0
  //#define MANUAL_Z_HOME_POS 402 // For delta: Distance between nozzle and print surface after homing.
#endif

// @section movement

/**
 * MOVEMENT SETTINGS
 */

#define HOMING_FEEDRATE {50*60, 50*60, 15*60, 0}  // set the homing speeds (mm/min)

// default settings
// XY motors: (steps * microsteps)/ (tooth-pitch * tooth-count) --> (200steps * 16) / (2mm * 15 teeth) = 106.66666666666667
// Z motor: (steps * microsteps) / (leadscrew pitch) --> (200steps * 4) / 2mm = 800
// E motor: 
#define DEFAULT_AXIS_STEPS_PER_UNIT   {106.6666666666667*2,106.6666666666667*2,1600,555}  // default steps per unit for Voxel8 gen3
#define DEFAULT_MAX_FEEDRATE          {90, 90, 10, 25}        // (mm/sec)
#define DEFAULT_MAX_ACCELERATION      {5000,5000,100,9000}    // X, Y, Z, E maximum start speed for accelerated moves. E default values are good for Skeinforge 40+, for older versions raise them a lot.

#define DEFAULT_ACCELERATION          1500    // X, Y, Z and E max acceleration in mm/s^2 for printing moves
#define DEFAULT_RETRACT_ACCELERATION  1500    // X, Y, Z and E max acceleration in mm/s^2 for retracts
#define DEFAULT_TRAVEL_ACCELERATION   1500    // X, Y, Z acceleration in mm/s^2 for travel (non printing) moves

// The speed change that does not require acceleration (i.e. the software might assume it can be done instantaneously)
#define DEFAULT_XYJERK                20.0    // (mm/sec)
#define DEFAULT_ZJERK                 0.4     // (mm/sec)
#define DEFAULT_EJERK                 20.0    // (mm/sec)

//=============================================================================
//============================= Additional Features ===========================
//=============================================================================

// @section more

// Custom M code points
#define CUSTOM_M_CODES
#if ENABLED(CUSTOM_M_CODES)
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    #define CUSTOM_M_CODE_SET_Z_PROBE_OFFSET 851
    #define Z_PROBE_OFFSET_RANGE_MIN -20
    #define Z_PROBE_OFFSET_RANGE_MAX 20
  #endif
#endif

// @section extras

// EEPROM
// The microcontroller can store settings in the EEPROM, e.g. max velocity...
// M500 - stores parameters in EEPROM
// M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).
// M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
//define this to enable EEPROM support
#define EEPROM_SETTINGS

#if ENABLED(EEPROM_SETTINGS)
  // To disable EEPROM Serial responses and decrease program space by ~1700 byte: comment this out:
  #define EEPROM_CHITCHAT // Please keep turned on if you can.
#endif

//==============================LCD and SD support=============================
// @section lcd

// Define your display language below. Replace (en) with your language code and uncomment.
// en, pl, fr, de, es, ru, bg, it, pt, pt-br, fi, an, nl, ca, eu, kana, kana_utf8, cn, test
// See also language.h
#define LANGUAGE_INCLUDE GENERATE_LANGUAGE_INCLUDE(en)


/*********************************************************************\
* R/C SERVO support
* Sponsored by TrinityLabs, Reworked by codexmas
**********************************************************************/

// Number of servos
//
// If you select a configuration below, this will receive a default value and does not need to be set manually
// set it manually if you have more servos than extruders and wish to manually control some
// leaving it undefined or defining as 0 will disable the servo subsystem
// If unsure, leave commented / disabled
//
//#define NUM_SERVOS 3 // Servo index starts with 0 for M280 command

// Servo Endstops
//
// This allows for servo actuated endstops, primary usage is for the Z Axis to eliminate calibration or bed height changes.
// Use M851 to set the Z probe vertical offset from the nozzle. Store that setting with M500.
//
//#define X_ENDSTOP_SERVO_NR 1
//#define Y_ENDSTOP_SERVO_NR 2
//#define Z_ENDSTOP_SERVO_NR 0
//#define SERVO_ENDSTOP_ANGLES {{0,0}, {0,0}, {70,0}} // X,Y,Z Axis Extend and Retract angles

// Servo deactivation
//
// With this option servos are powered only during movement, then turned off to prevent jitter.
//#define DEACTIVATE_SERVOS_AFTER_MOVE

#if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
  // Delay (in microseconds) before turning the servo off. This depends on the servo speed.
  // 300ms is a good value but you can try less delay.
  // If the servo can't reach the requested position, increase it.
  #define SERVO_DEACTIVATION_DELAY 300
#endif

/**********************************************************************\
 * Support for a filament diameter sensor
 * Also allows adjustment of diameter at print time (vs  at slicing)
 * Single extruder only at this point (extruder 0)
 *
 * Motherboards
 * 34 - RAMPS1.4 - uses Analog input 5 on the AUX2 connector
 * 81 - Printrboard - Uses Analog input 2 on the Exp1 connector (version B,C,D,E)
 * 301 - Rambo  - uses Analog input 3
 * Note may require analog pins to be defined for different motherboards
 **********************************************************************/


#include "Configuration_adv.h"
#include "thermistortables.h"

#endif //CONFIGURATION_H
