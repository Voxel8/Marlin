/*
 * E1_Defs.h
 *
 * Definitions for controlling E1 Stepper Driver
 *
 */

#ifndef E1_DEFS_H
#define E1_DEFS_H

#include <avr/io.h>

// Port Definitions
#define STEP_PORT       (PORTC)
#define DIR_PORT        (PORTL)
#define EN_PORT         (PORTA)

#define STEP_DDR        (DDRC)
#define DIR_DDR         (DDRL)
#define EN_DDR          (DDRA)

#define E1_MS_PORT      (PORTK)
#define E1_MS_DDR       (DDRK)

// Pin Definitions
#define E1_STEP         (0x10)
#define E1_DIR          (0x80)
#define E1_EN           (0x08)

#define E1_MS1          (0x02)
#define E1_MS2          (0x40)

#endif // E1_DEFS_H

