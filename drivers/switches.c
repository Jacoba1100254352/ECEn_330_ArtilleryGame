#include "switches.h"
#include "stdio.h"

#include "xil_io.h"
#include "xparameters.h"

#define DATA_OFFSET 0x00
#define TRI_OFFSET 0x04
#define PINS_INPUT 0xF
#define PINS_OUTPUT 0x0

// Purpose: Reads the switch value status
// Arguments: Offset value
// Return Values: Status of switches
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + offset);
}

// Purpose: Writes values to the board
// Arguments: Offset value and value to be written
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
}

// Initializes the SWITCHES driver software and hardware.
void switches_init() { writeRegister(TRI_OFFSET, PINS_INPUT); }

// Returns the current value of all 4 switches as the lower 4 bits of the
// returned value. bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
uint8_t switches_read() { return readRegister(DATA_OFFSET); }