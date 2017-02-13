/***********************************************************************************
*
* Intended for use with MCP4725 12-bit DAC
*
***********************************************************************************/
#include "Regulator.h"
#include "MCP4725.h"
#include "./language.h"

#if ENABLED(E_REGULATOR)

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

//===========================================================================
//============================ Private Variables ============================
//===========================================================================

static float current_target_pressure = 0;
static bool  regulator_active = false;
static bool  protectionsActive = true;
//===========================================================================
//====================== Private Functions Prototypes =======================
//===========================================================================

static void _regulator_leak_error();
static void _regulator_runaway_error();
static void pressure_protection(float pressure, float target_pressure);

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

/**
 * Set the output pressure (in psi) of the regulator.
 * @param desired_pressure  The pressure that the function is attempting to 
 *                          reach.
 */
void Regulator__SetOutputPressure(float desired_pressure) {
    uint16_t digital_val = 0;
    regulator_active = true;
    // Set current pressure for Regulator__Update()
    current_target_pressure = desired_pressure;
    // Set to zero
    if (desired_pressure <= (REG_OFFSET + REG_HYSTERESIS)) {
        digital_val = 0;
    }
    // Increasing pressure
    else if (desired_pressure > pressureRegulator()) {
        // Add hysteresis value to desired pressure
        digital_val = (uint16_t)(BITS_PER_PSI *
                                 (desired_pressure - REG_OFFSET + REG_HYSTERESIS));
    }
    // Decreasing pressure
    else if (desired_pressure < pressureRegulator()) {
        // Subtract hysteresis value from desired pressure
        digital_val = (uint16_t)(BITS_PER_PSI *
                                 (desired_pressure - REG_OFFSET - REG_HYSTERESIS));
    }
    
    // Write value to DAC
    DAC_write(MCP4725_I2C_ADDRESS, digital_val);
}


/**
 * Updates the protection function, called regularly in the main loop.
 */
void Regulator__Update() {
  if (protectionsActive) {
    pressure_protection(current_target_pressure, pressureRegulator());
  }
}

 /** 
  * Enables or disables pressure protections
  * @value     true = enable, false = no check.
  */
  void Regulator__SetPressureProtections(bool value) {
    protectionsActive = value;
  }

//===========================================================================
//============================ Private Functions ============================
//===========================================================================

/** 
 * Error handler when marlin detects a missing pressure regulator
 * @param serial_msg  The message to be displayed when the error occurs
 * @param sled_status Sets whether the pneumatics is present or not.
 */
static void _regulator_error_handler(const char *serial_msg, float pressure) {
    quickStop();
    Regulator__SetOutputPressure(0);
    disable_all_heaters();
    disable_all_steppers();
    if (pressure >= REGULATOR_NOT_PRESENT_VALUE) {
      SERIAL_PROTOCOLPGM("// action:message ");
      serialprintPGM(PSTR(MSG_T_PNEUMATICS_REMOVED));
    } else {
      SERIAL_PROTOCOLPGM("// action:message ");
      serialprintPGM(serial_msg);
    }
    SERIAL_EOL;
    SERIAL_PROTOCOLLNPGM("// action:cancel");
    regulator_active = false;
}

/** 
 * Allows us to determine if the pressure is too high or low, indicating
 * a serious issue. Checks the target pressure vs. the desired, and 
 * throws an error if outside the bounds for too long.
 * @param target_pressure  The pressure you're attempting to reach
 * @param pressure         The actual current pressure 
 */
static void pressure_protection(float target_pressure, float pressure) {
  static float prev_target_pressure = 0;
  static millis_t regulatorTimer = 0;
  bool regulatorUnder = false;
  bool regulatorOver = false;
  // This is set the first time the function is called to initialize it.
  if (regulatorTimer == 0) regulatorTimer = millis();

  // Only run protections if the pneumatics system is running
  if (regulator_active) {

    // Is output pressure more than what is available? If so, we throw an error.
    if ((pressureRegulator() >= pressurePneumatic()) &&
        (pressurePneumatic() >= REGULATOR_LOW_P) &&
        (pressureRegulator() >= REGULATOR_NOT_PRESENT_VALUE)) {
      _regulator_error_handler(PSTR(MSG_T_PNEUMATICS_EREG_ABOVE_PUMP), pressure);
    }
  
    // Calculate the conditions where we may enter an error. This is split into
    // two bands, to accomodate larger variable in higher pressures.
    if (target_pressure <= REGULATOR_BAND_CROSSOVER_PSI) {
      regulatorUnder =
          (pressure <= (target_pressure - REGULATOR_PROTECTION_BAND_LOW));
      regulatorOver =
          (pressure >= (target_pressure + REGULATOR_PROTECTION_BAND_LOW));
    } else {
      regulatorUnder =
          (pressure <= (target_pressure - REGULATOR_PROTECTION_BAND_HIGH));
      regulatorOver =
          (pressure >= (target_pressure + REGULATOR_PROTECTION_BAND_HIGH));
    }
    // Check to see if we're outside the bands
    if ((regulatorOver || regulatorUnder)) {
      //  If the conditions have been met for 5 seconds without going within
      //  their bounds
      if (millis() > (regulatorTimer + REGULATOR_PROTECTION_TIME_S * 1000UL)) {
        // Assuming the sled is present, display the target and actual pressure.
        SERIAL_PROTOCOLPGM(" Target Pressure ");
        SERIAL_PROTOCOL(target_pressure);
        SERIAL_PROTOCOLPGM(" Actual Pressure ");
        SERIAL_PROTOCOL(pressure);
        SERIAL_EOL;
  
        // The appropriate error is chosen here. If the pressure is much too high,
        // we assume the sled is removed. If it's not too high, runaway or leak
        // error handlers are called for being above or below the band, respectively.
        if (pressure >= REGULATOR_NOT_PRESENT_VALUE) {
          _regulator_error_handler(PSTR(MSG_T_PNEUMATICS_REMOVED), pressure);
        } else if (regulatorOver) {
          _regulator_error_handler(PSTR(MSG_T_PNEUMATICS_RUNAWAY), pressure);
        } else {
          _regulator_error_handler(PSTR(MSG_T_PNEUMATICS_LEAK), pressure);
        }
    
        // This is for resetting the timer after triggering an error. This 
        // prevents the error messages from being spammed while the pressure goes 
        // back to 0. If the regulator is over or under the pressure targets and 
        // hasn't gotten to the prerequisite regulator protection time, 
        // regulatorTimer is not reset to millis()
        regulatorTimer = millis();
      } 
    } else {
      // If we're within our pressure bounds, assume that the sled is present and
      // reset the timer.
      regulatorTimer = millis();
    }
  } else {
    regulatorTimer = millis();
  }
}

#endif  // ENABLED(E_REGULATOR)
