/**
 * MCodeUtilities.h - Miscellanous Functions used by MCodes
 * Author: Dan Thompson (danthompson41@gmail.com)
 * Date: 4/6/2016
 * Utilities used by Mcodes to perform their functions.
 * Refactor from keeping them in Marlin_Main.cpp
 * Copyright (C) 2016 Voxel8
 */
#ifndef M_CODE_UTILITIES_H_
#define M_CODE_UTILITIES_H_

#if ENABLED(M100_FREE_MEMORY_WATCHER)
  void gcode_M100();
#endif

#if ENABLED(SDSUPPORT)
  CardReader card;
#endif

// This bool is referenced in many places, and will cause the printer to not 
// operate if set to false
bool Running = true;


// This bool will cause some error conditions to kill the printer if activated, 
// instead of trying to continue normal operation.
bool SafetyCriticalSection = false;

#define START_SAFETY_CRITICAL_SECTION (SafetyCriticalSection = true)
#define END_SAFETY_CRITICAL_SECTION (SafetyCriticalSection = false)

uint8_t marlin_debug_flags = DEBUG_INFO|DEBUG_ERRORS;

static float feedrate = 1500.0, saved_feedrate;
float current_position[NUM_AXIS] = { 0.0 };
static float destination[NUM_AXIS] = { 0.0 };
static uint16_t bedlevelprobes[9] = { 0.0 };
bool axis_known_position[3] = { false };
bool min_software_endstops_enabled[Z_AXIS + 1] = { false };
bool max_software_endstops_enabled[Z_AXIS + 1] = { false };

static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

static char *current_command, *current_command_args;
static int cmd_queue_index_r = 0;
static int cmd_queue_index_w = 0;
static int commands_in_queue = 0;
static char command_queue[BUFSIZE][MAX_CMD_SIZE];

const float homing_feedrate[] = HOMING_FEEDRATE;
bool axis_relative_modes[] = AXIS_RELATIVE_MODES;
int feedrate_multiplier = 100; //100->1 200->2
int saved_feedrate_multiplier;
int extruder_multiplier[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(100);
bool volumetric_enabled = false;
float filament_size[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(DEFAULT_NOMINAL_FILAMENT_DIA);
float volumetric_multiplier[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(1.0);
float home_offset[3] = { 0 };
float min_pos[3] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS };
float max_pos[3] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };

uint8_t active_extruder = 0;
int fanSpeed = 0;
bool cancel_heatup = false;

const char errormagic[] PROGMEM = "Error:";
const char echomagic[] PROGMEM = "echo:";
const char axis_codes[NUM_AXIS] = {'X', 'Y', 'Z', 'E'};

static bool relative_mode = false;  //Determines Absolute or Relative Coordinates
static char serial_char;
static int serial_count = 0;
static boolean comment_mode = false;
static char *seen_pointer; ///< A pointer to find chars in the command string (X, Y, Z, E, etc.)
const char* queued_commands_P= NULL; /* pointer to the current line in the active sequence of commands, or NULL when none */
const int sensitive_pins[] = SENSITIVE_PINS; ///< Sensitive pin list for M42
// Inactivity shutdown
millis_t previous_cmd_ms = 0;
static millis_t max_inactive_time = 0;
static millis_t stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME * 1000L;
millis_t print_job_start_ms = 0; ///< Print job start time
millis_t print_job_stop_ms = 0;  ///< Print job stop time
static uint8_t target_extruder;
bool no_wait_for_cooling = true;
bool target_direction;

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
  int xy_travel_speed = XY_TRAVEL_SPEED;
  float zprobe_zoffset = Z_PROBE_OFFSET_FROM_EXTRUDER;
#endif

#if ENABLED(Z_DUAL_ENDSTOPS) && DISABLED(DELTA)
  float z_endstop_adj = 0;
#endif

#if ENABLED(E_REGULATOR)
  float regulator_setpoint = 0;
#endif

// Extruder offsets
#if EXTRUDERS > 1
  #ifndef EXTRUDER_OFFSET_X
    #define EXTRUDER_OFFSET_X { 0 }
  #endif
  #ifndef EXTRUDER_OFFSET_Y
    #define EXTRUDER_OFFSET_Y { 0 }
  #endif
  #ifndef EXTRUDER_OFFSET_Z
    #define EXTRUDER_OFFSET_Z { 0 }
  #endif
  float extruder_offset[][EXTRUDERS] = {
    EXTRUDER_OFFSET_X,
    EXTRUDER_OFFSET_Y,
    EXTRUDER_OFFSET_Z
    #if ENABLED(DUAL_X_CARRIAGE)
      , { 0 } // supports offsets in XYZ plane
    #endif
  };
#endif

#if HAS_SERVO_ENDSTOPS
  const int servo_endstop_id[] = SERVO_ENDSTOP_IDS;
  const int servo_endstop_angle[][2] = SERVO_ENDSTOP_ANGLES;
#endif

#if ENABLED(BARICUDA)
  int ValvePressure = 0;
  int EtoPPressure = 0;
#endif

#if ENABLED(FWRETRACT)

  bool autoretract_enabled = false;
  bool retracted[EXTRUDERS] = { false };
  bool retracted_swap[EXTRUDERS] = { false };

  float retract_length = RETRACT_LENGTH;
  float retract_length_swap = RETRACT_LENGTH_SWAP;
  float retract_feedrate = RETRACT_FEEDRATE;
  float retract_zlift = RETRACT_ZLIFT;
  float retract_recover_length = RETRACT_RECOVER_LENGTH;
  float retract_recover_length_swap = RETRACT_RECOVER_LENGTH_SWAP;
  float retract_recover_feedrate = RETRACT_RECOVER_FEEDRATE;

#endif // FWRETRACT

#if ENABLED(ULTIPANEL) && HAS_POWER_SWITCH
  bool powersupply =
    #if ENABLED(PS_DEFAULT_OFF)
      false
    #else
      true
    #endif
  ;
#endif

#if ENABLED(DELTA)

  #define TOWER_1 X_AXIS
  #define TOWER_2 Y_AXIS
  #define TOWER_3 Z_AXIS

  float delta[3] = { 0 };
  #define SIN_60 0.8660254037844386
  #define COS_60 0.5
  float endstop_adj[3] = { 0 };
  // these are the default values, can be overriden with M665
  float delta_radius = DELTA_RADIUS;
  float delta_tower1_x = -SIN_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_1); // front left tower
  float delta_tower1_y = -COS_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_1);
  float delta_tower2_x =  SIN_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_2); // front right tower
  float delta_tower2_y = -COS_60 * (delta_radius + DELTA_RADIUS_TRIM_TOWER_2);
  float delta_tower3_x = 0;                                                    // back middle tower
  float delta_tower3_y = (delta_radius + DELTA_RADIUS_TRIM_TOWER_3);
  float delta_diagonal_rod = DELTA_DIAGONAL_ROD;
  float delta_diagonal_rod_trim_tower_1 = DELTA_DIAGONAL_ROD_TRIM_TOWER_1;
  float delta_diagonal_rod_trim_tower_2 = DELTA_DIAGONAL_ROD_TRIM_TOWER_2;
  float delta_diagonal_rod_trim_tower_3 = DELTA_DIAGONAL_ROD_TRIM_TOWER_3;
  float delta_diagonal_rod_2_tower_1 = sq(delta_diagonal_rod + delta_diagonal_rod_trim_tower_1);
  float delta_diagonal_rod_2_tower_2 = sq(delta_diagonal_rod + delta_diagonal_rod_trim_tower_2);
  float delta_diagonal_rod_2_tower_3 = sq(delta_diagonal_rod + delta_diagonal_rod_trim_tower_3);
  //float delta_diagonal_rod_2 = sq(delta_diagonal_rod);
  float delta_segments_per_second = DELTA_SEGMENTS_PER_SECOND;
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    int delta_grid_spacing[2] = { 0, 0 };
    float bed_level[AUTO_BED_LEVELING_GRID_POINTS][AUTO_BED_LEVELING_GRID_POINTS];
  #endif
#else
  static bool home_all_axis = true;
#endif

#if ENABLED(SCARA)
  float delta_segments_per_second = SCARA_SEGMENTS_PER_SECOND;
  static float delta[3] = { 0 };
  float axis_scaling[3] = { 1, 1, 1 };    // Build size scaling, default to 1
#endif

#if ENABLED(FILAMENT_SENSOR)
  //Variables for Filament Sensor input
  float filament_width_nominal = DEFAULT_NOMINAL_FILAMENT_DIA;  //Set nominal filament width, can be changed with M404
  bool filament_sensor = false;  //M405 turns on filament_sensor control, M406 turns it off
  float filament_width_meas = DEFAULT_MEASURED_FILAMENT_DIA; //Stores the measured filament diameter
  signed char measurement_delay[MAX_MEASUREMENT_DELAY+1];  //ring buffer to delay measurement  store extruder factor after subtracting 100
  int delay_index1 = 0;  //index into ring buffer
  int delay_index2 = -1;  //index into ring buffer - set to -1 on startup to indicate ring buffer needs to be initialized
  float delay_dist = 0; //delay distance counter
  int meas_delay_cm = MEASUREMENT_DELAY_CM;  //distance delay setting
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
   static bool filrunoutEnqueued = false;
#endif

#if ENABLED(SDSUPPORT)
  static bool fromsd[BUFSIZE];
#endif

#if HAS_SERVOS
  Servo servo[NUM_SERVOS];
#endif

#ifdef CHDK
  unsigned long chdkHigh = 0;
  boolean chdkActive = false;
#endif

#if ENABLED(PID_ADD_EXTRUSION_RATE)
  int lpq_len = 20;
#endif


void gcode_get_destination();
bool code_seen(char code);
float code_value(void);

 #endif // M_CODE_UTILITIES_H_