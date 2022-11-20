#include "intervalTimer.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdio.h>

#define TIMER_0_NUMBER 0
#define TIMER_1_NUMBER 1
#define TIMER_2_NUMBER 2

#define TIMER_0_GPIO_DEVICE_BASE_ADDRESS XPAR_AXI_TIMER_0_BASEADDR
#define TIMER_1_GPIO_DEVICE_BASE_ADDRESS XPAR_AXI_TIMER_1_BASEADDR
#define TIMER_2_GPIO_DEVICE_BASE_ADDRESS XPAR_AXI_TIMER_2_BASEADDR

#define TIMER_CLOCK_FREQ_HZ XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;

#define TCSR0_OFFSET 0x00
#define TCSR1_OFFSET 0x10
#define TLR0_OFFSET 0x04
#define TLR1_OFFSET 0x14
#define TCR0_OFFSET 0x08
#define TCR1_OFFSET 0x18

#define CASC_MASK 0x0800
#define UTD_MASK 0x0002
#define ENT_MASK 0x0080
#define ENIT_MASK 0x0040
#define T0INT 0x0100
#define LOAD_MASK 0x0020
#define RELOAD_MASK 0x0020
#define AUTO_RELOAD 0x0010
#define CASC_UTD0_INIT 0x00000810
#define SHIFT_32 32

#define CLEAR 0

// Purpose: Reads the switch value status
// Arguments: Offset value
// Return Values: Status of switches
static uint32_t readRegister(uint8_t timerNumber, uint32_t offset) {
  // Switch addresses based on timer number
  if (timerNumber == INTERVAL_TIMER_0)
    return Xil_In32(TIMER_0_GPIO_DEVICE_BASE_ADDRESS + offset);
  else if (timerNumber == INTERVAL_TIMER_1)
    return Xil_In32(TIMER_1_GPIO_DEVICE_BASE_ADDRESS + offset);
  else if (timerNumber == INTERVAL_TIMER_2)
    return Xil_In32(TIMER_2_GPIO_DEVICE_BASE_ADDRESS + offset);
  else
    return CLEAR;
}

// Purpose: Writes values to the board
// Arguments: Offset value and value to be written
static void writeRegister(uint8_t timerNumber, uint32_t offset,
                          uint32_t value) {
  // Switch addresses based on timer number
  if (timerNumber == INTERVAL_TIMER_0)
    Xil_Out32(TIMER_0_GPIO_DEVICE_BASE_ADDRESS + offset, value);
  else if (timerNumber == INTERVAL_TIMER_1)
    Xil_Out32(TIMER_1_GPIO_DEVICE_BASE_ADDRESS + offset, value);
  else if (timerNumber == INTERVAL_TIMER_2)
    Xil_Out32(TIMER_2_GPIO_DEVICE_BASE_ADDRESS + offset, value);
  else
    printf("Timer Number is not available in writeRegister function");
}

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts up
// 2. Initialize both LOAD registers with zeros
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountUp(uint32_t timerNumber) {
  writeRegister(timerNumber, TCSR0_OFFSET, CLEAR);
  writeRegister(timerNumber, TCSR1_OFFSET, CLEAR);
  writeRegister(timerNumber, TCSR0_OFFSET, CASC_UTD0_INIT);

  intervalTimer_reload(timerNumber);
}

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts down
//  - The timer automatically reloads when reaching zero
// 2. Initialize LOAD registers with appropriate values, given the `period`.
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountDown(uint32_t timerNumber, double period) {
  uint64_t ticks = period * TIMER_CLOCK_FREQ_HZ;
  uint32_t upper = ticks >> SHIFT_32;
  uint32_t lower = ticks;

  writeRegister(timerNumber, TCSR0_OFFSET, CLEAR);
  writeRegister(timerNumber, TCSR1_OFFSET, CLEAR);
  writeRegister(timerNumber, TCSR0_OFFSET, CASC_MASK | AUTO_RELOAD | UTD_MASK);
  writeRegister(timerNumber, TLR1_OFFSET, upper);
  writeRegister(timerNumber, TLR0_OFFSET, lower);

  intervalTimer_reload(timerNumber);
}

// This function starts the interval timer running.
// If the interval timer is already running, this function does nothing.
// timerNumber indicates which timer should start running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_start(uint32_t timerNumber) {
  int32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  writeRegister(timerNumber, TCSR0_OFFSET, data | ENT_MASK);
}

// This function stops a running interval timer.
// If the interval time is currently stopped, this function does nothing.
// timerNumber indicates which timer should stop running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_stop(uint32_t timerNumber) {
  int32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  writeRegister(timerNumber, TCSR0_OFFSET, data & ~ENT_MASK);
}

// This function is called whenever you want to reload the Counter values
// from the load registers.  For a count-up timer, this will reset the
// timer to zero.  For a count-down timer, this will reset the timer to
// its initial count-down value.  The load registers should have already
// been set in the appropriate `init` function, so there is no need to set
// them here.  You just need to enable the load (and remember to disable it
// immediately after otherwise you will be loading indefinitely).
void intervalTimer_reload(uint32_t timerNumber) {
  uint32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  writeRegister(timerNumber, TCSR0_OFFSET, data | RELOAD_MASK);
  writeRegister(timerNumber, TCSR0_OFFSET, data & ~RELOAD_MASK);
  // data = readRegister(timerNumber, TCSR1_OFFSET);
  writeRegister(timerNumber, TCSR1_OFFSET, data | RELOAD_MASK);
  writeRegister(timerNumber, TCSR1_OFFSET, data & ~RELOAD_MASK);
}

// Use this function to ascertain how long a given timer has been running.
// Note that it should not be an error to call this function on a running timer
// though it usually makes more sense to call this after intervalTimer_stop()
// has been called. The timerNumber argument determines which timer is read.
// In cascade mode you will need to read the upper and lower 32-bit registers,
// concatenate them into a 64-bit counter value, and then perform the conversion
// to a double seconds value.
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  uint64_t upper = readRegister(timerNumber, TCR1_OFFSET);
  uint64_t lower = readRegister(timerNumber, TCR0_OFFSET);
  uint64_t total = upper << SHIFT_32 | lower;
  double seconds = (double)total / TIMER_CLOCK_FREQ_HZ;
  return seconds;
}

// Enable the interrupt output of the given timer.
void intervalTimer_enableInterrupt(uint8_t timerNumber) {
  uint32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  writeRegister(timerNumber, TCSR0_OFFSET, data | ENIT_MASK);
}

// Disable the interrupt output of the given timer.
void intervalTimer_disableInterrupt(uint8_t timerNumber) {
  uint32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  writeRegister(timerNumber, TCSR0_OFFSET, data & ~ENIT_MASK);
}

// Acknowledge the rollover to clear the interrupt output.
void intervalTimer_ackInterrupt(uint8_t timerNumber) {
  uint32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  writeRegister(timerNumber, TCSR0_OFFSET, data | T0INT);
}