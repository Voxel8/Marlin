/*
 * E_defs.h
 *
 * Definitions for controlling E0 and E1 Stepper Driver
 *
 */

#ifndef E_DEFS_H
#define E_DEFS_H

#include <avr/io.h>

// Port Definitions
#define E_STEP_PORT     (PORTC)
#define E_DIR_PORT      (PORTL)
#define E_EN_PORT       (PORTA)

#define E_STEP_DDR      (DDRC)
#define E_DIR_DDR       (DDRL)
#define E_EN_DDR        (DDRA)

#define E_MS_PORT       (PORTK)
#define E_MS_DDR        (DDRK)

// Pin Definitions
#define E0_STEP         (0x08)
#define E0_DIR          (0x40)
#define E0_EN           (0x10)

#define E0_MS1          (0x08)
#define E0_MS2          (0x10)

#define E1_STEP         (0x10)
#define E1_DIR          (0x80)
#define E1_EN           (0x08)

#define E1_MS1          (0x02)
#define E1_MS2          (0x04)

#endif // E_DEFS_H

