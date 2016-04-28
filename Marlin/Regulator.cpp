/***********************************************************************************
*
* Intended for use with MCP4725 12-bit DAC
*
***********************************************************************************/
#include "Regulator.h"
#include "MCP4725.h"
#include "temperature.h"

#if ENABLED(E_REGULATOR)

//===========================================================================
//=============================== Definitions ===============================
//===========================================================================

// Relevant Pins
// Voxel8 C2
// #define PNEUMATIC_PUMP_PIN    7   // Pump driver
// #define PNEUMATIC_PIN         5   // A5 Input analog tank pressure reading
// #define PNEUMATIC_CART1_PIN   4   // A4 Cartridge 2 analog pressure reading

#define REGULATOR_PROTECTION_TIME_S (5)
#define REGULATOR_PROTECTION_BAND   (4)

//===========================================================================
//============================ Private Variables ============================
//===========================================================================

static float current_target_pressure = 0;

//===========================================================================
//====================== Private Functions Prototypes =======================
//===========================================================================

static void _regulator_leak_error();
static void _regulator_runaway_error();
static void pressure_protection(float pressure, float target_pressure);

//===========================================================================
//============================= Public Functions ============================
//===========================================================================

// Set the output pressure (in psi) of the regulator.
<<<<<<< 7d30f7313b0d45f4e940d82533faaa473e9181db
void Regulator__SetOutputPressure(float desired_pressure) {
    uint16_t digital_val = 0;
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

void Regulator__Update() {
  pressure_protection(current_target_pressure, pressureRegulator());
}

//===========================================================================
//============================ Private Functions ============================
//===========================================================================

static void _regulator_leak_error() {
  SERIAL_PROTOCOL(
      "Pressure Leak Error. Canceling print, check the pressure hose");
  SERIAL_EOL;
  SERIAL_ECHOLN("// action:cancel");
  SERIAL_EOL;
  Regulator__SetOutputPressure(0.0);
}

static void _regulator_runaway_error() {
  SERIAL_PROTOCOL("Runaway Pressure Error. Canceling print.");
  SERIAL_EOL;
  SERIAL_ECHOLN("// action:cancel");
  SERIAL_EOL;
  Regulator__SetOutputPressure(0.0);
}

static void pressure_protection(float target_pressure, float pressure) {
  static const int period_seconds = REGULATOR_PROTECTION_TIME_S;
  static float prev_target_pressure = 0;
  static millis_t regulatorTimer = 0;

  if (regulatorTimer == 0) regulatorTimer = millis();
  bool regulatorUnder =
      (pressure <= (target_pressure - REGULATOR_PROTECTION_BAND));
  bool regulatorOver =
      (pressure >= (target_pressure + REGULATOR_PROTECTION_BAND));

  if (regulatorOver || regulatorUnder) {
    if (millis() > (regulatorTimer + REGULATOR_PROTECTION_TIME_S * 1000UL)) {
      SERIAL_PROTOCOL(" Target Pressure ");
      SERIAL_PROTOCOL(target_pressure);
      SERIAL_PROTOCOL(" Actual Pressure ");
      SERIAL_PROTOCOL(pressure);
      SERIAL_EOL;
      if (regulatorOver) {
        _regulator_runaway_error();
      } else {
        _regulator_leak_error();
      }
      regulatorTimer = millis();
    }
  } else {
    regulatorTimer = millis();
  }
}

#endif  // ENABLED(E_REGULATOR)