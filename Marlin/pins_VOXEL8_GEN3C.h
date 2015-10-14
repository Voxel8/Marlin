/**
 * Rambo pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH             true

// Servo support
#ifdef NUM_SERVOS
  #define SERVO0_PIN            22  //motor header MX1
  #if NUM_SERVOS > 1
    #define SERVO1_PIN          23  //Motor header MX2
    #if NUM_SERVOS > 2
      #define SERVO2_PIN        24  //Motor header MX3
      #if NUM_SERVOS > 3
        #define SERVO2_PIN      9   //pwm header pin 5
      #endif
    #endif
  #endif
#endif

/*************************
	  STEPPER DRIVER PINS
*************************/
#undef X_MS1_PIN
#undef X_MS2_PIN
#undef Y_MS1_PIN
#undef Y_MS2_PIN
#undef Z_MS1_PIN
#undef Z_MS2_PIN
#undef E0_MS1_PIN
#undef E0_MS2_PIN
#undef E1_MS1_PIN
#undef E1_MS2_PIN
 
#define X_STEP_PIN              37  // Actuate X stepper driver
#define X_DIR_PIN               48  // X Stepper driver direction
#define X_MIN_PIN               24  // X min limit switch
#define X_MAX_PIN               76  // X max limit switch
#define X_ENABLE_PIN            29  // Enable X stepper driver
#define X_MS1_PIN               40  // X stepper driver microstep select 1
#define X_MS2_PIN               41  // X stepper driver microstep select 2

#define Y_STEP_PIN              36  // Actuate Y stepper driver
#define Y_DIR_PIN               49  // Y Stepper driver direction
#define Y_MIN_PIN               23  // Y min limit switch
#define Y_MAX_PIN               77  // Y max limit switch
#define Y_ENABLE_PIN            28  // Enable Y stepper driver
#define Y_MS1_PIN               69  // Y stepper driver microstep select 1
#define Y_MS2_PIN               39  // Y stepper driver microstep select 2

#define Z_STEP_PIN              35  // Actuate Z stepper driver
#define Z_DIR_PIN               47  // Z Stepper driver direction
#define Z_MIN_PIN               73  // Z min limit switch
#define Z_MAX_PIN               30  // Z max limit switch
#define Z_ENABLE_PIN            27  // Enable Z stepper driver
#define Z_MS1_PIN               68  // Z stepper driver microstep select 1
#define Z_MS2_PIN               67  // Z stepper driver microstep select 2

/************************
DIGITAL POTENTIOMETER PINS
*************************/
// Currently used for limiting motor current
#undef DIGIPOTSS_PIN
#define DIGIPOTSS_PIN           38  // Chip Select for digipot SPI communication
#define DIGIPOT_CHANNELS        {4,5,3,0,1}  // X Y Z E0 E1 digipot channels to stepper driver mapping

/*************************
    HEATED BED PINS
*************************/
#define BED_AVAIL_PIN           82  // From key circuit
#define HEATER_BED_PIN          4   // Drive the heated bed
#define TEMP_BED_PIN            2   // Heated bed thermistor

/*************************
    CARTRIDGE PINS
*************************/
#define CART0_SIG0_PIN          5   // PWM Capable
#define CART0_SIG1_PIN          46  // PWM Capable
#define CART0_SIG2_PIN          74
#define CART1_SIG0_PIN          45  // PWM Capable
#define CART1_SIG1_PIN          75  // Interrupt Capable
#define CART1_SIG2_PIN          85

/*************************
	     FFF PINS
*************************/

#define HEATER_0_PIN            CART0_SIG0_PIN
#define HEATER_1_PIN            -1 // Set to CART1_SIG0_PIN to use FFF in CART1

/*************************
  TEMPERATURE SENSE PINS
*************************/
#define TEMP_0_PIN              6   // A6 Input Cart0 Therm
#define TEMP_1_PIN              1   // A1 Input Cart1 Therm
#define TEMP_2_PIN              -1

/*************************
    PNEUMATIC PINS
*************************/
#ifdef PNEUMATICS
  #define PNEUMATIC_PUMP_PIN    7   // Pump driver
  #define PNEUMATIC_PIN         5   // A5 Input analog tank pressure reading
  #define REGULATOR_PIN         3   // A3 Input analog output pressure reading
  #define PNEUMATIC_CART1_PIN   4   // A4 Cartridge 2 analog pressure reading
#endif

/*************************
    SOLENOID PINS
*************************/
#define SOL0_PIN                -1 // Set to CART0_SIG0_PIN for pneumatics in CART0
#define SOL1_PIN                CART1_SIG0_PIN


#define E0_STEP_PIN             34
#define E0_DIR_PIN              43
#define E0_ENABLE_PIN           26
#define E0_MS1_PIN              65
#define E0_MS2_PIN              66

#define E1_STEP_PIN             33
#define E1_DIR_PIN              42
#define E1_ENABLE_PIN           25
#define E1_MS1_PIN              63
#define E1_MS2_PIN              64

/**********************************************************
  Fan Pins
  Fan_0 8
  Fan_1 6
  Fan_2 2
***********************************************************/
#define LED_PIN                 13
#define FAN_PIN                 CART0_SIG2_PIN  // FFF fan toggle
#define FAN_CHASSIS_TOP_PIN     2
#define FAN_CHASSIS_BOT_PIN     44

/*************************
	  ADMINISTRATIVE PINS
*************************/
#define V_MONITOR_PIN           79  //! Monitor voltage to print engine
#define PS_ON_PIN               4
#define KILL_PIN                -1  //80 with Smart Controller LCD
#define SUICIDE_PIN             -1  //PIN that has to be turned on right after start, to keep power flowing.
#define SDPOWER                 -1
#define SDSS                    53

/*************************
	  FILAMENT SENSE PINS
*************************/
#ifdef FILAMENT_SENSOR
  //Filip added pin for Filament sensor analog input 
  #define FILMNT_SENS_PIN       80  //! Monitor filament low
#endif
