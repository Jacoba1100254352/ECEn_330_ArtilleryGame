/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

#ifndef LEDS
#define LEDS

#include <stdint.h>

// This will init the GPIO hardware so you can write to the 4 LEDs (LD3 - LD0)
int32_t leds_init();

// This write the lower 4 bits of ledValue to the LEDs.
// LED3 gets bit3 and so forth.
// '1' = illuminated.
// '0' = off.
void leds_write(uint8_t ledValue);

// Return the current value of the LEDs
uint8_t leds_read();

// Theis controls the LED LD4 attached to MIO 7 on the ZYBO board.
void leds_writeLd4(uint8_t ledValue);

// This blinks all of the LEDs for several seconds to provide a visual test of
// the code.
void leds_runTest();

#endif /* LEDS */
