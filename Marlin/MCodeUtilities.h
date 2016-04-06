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

// This bool is referenced in many places, and will cause the printer to not 
// operate if set to false
bool Running = true;


// This bool will cause some error conditions to kill the printer if activated, 
// instead of trying to continue normal operation.
bool SafetyCriticalSection = false;

#define START_SAFETY_CRITICAL_SECTION (SafetyCriticalSection = true)
#define END_SAFETY_CRITICAL_SECTION (SafetyCriticalSection = false)

uint8_t marlin_debug_flags = DEBUG_INFO|DEBUG_ERRORS;

float feedrate = 1500.0, saved_feedrate;
float current_position[NUM_AXIS] = { 0.0 };
float destination[NUM_AXIS] = { 0.0 };
uint16_t bedlevelprobes[9] = { 0.0 };
bool axis_known_position[3] = { false };
bool min_software_endstops_enabled[Z_AXIS + 1] = { false };
bool max_software_endstops_enabled[Z_AXIS + 1] = { false };

long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

char *current_command, *current_command_args;
int cmd_queue_index_r = 0;
int cmd_queue_index_w = 0;
int commands_in_queue = 0;
char command_queue[BUFSIZE][MAX_CMD_SIZE];

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

bool relative_mode = false;  //Determines Absolute or Relative Coordinates
char serial_char;
int serial_count = 0;
boolean comment_mode = false;
char *seen_pointer; ///< A pointer to find chars in the command string (X, Y, Z, E, etc.)
const char* queued_commands_P= NULL; /* pointer to the current line in the active sequence of commands, or NULL when none */
const int sensitive_pins[] = SENSITIVE_PINS; ///< Sensitive pin list for M42
// Inactivity shutdown
millis_t previous_cmd_ms = 0;
millis_t max_inactive_time = 0;
millis_t stepper_inactive_time = DEFAULT_STEPPER_DEACTIVE_TIME * 1000L;
millis_t print_job_start_ms = 0; ///< Print job start time
millis_t print_job_stop_ms = 0;  ///< Print job stop time
uint8_t target_extruder;
bool no_wait_for_cooling = true;
bool target_direction;


void gcode_get_destination();
bool code_seen(char code);
float code_value(void);

 #endif // M_CODE_UTILITIES_H_