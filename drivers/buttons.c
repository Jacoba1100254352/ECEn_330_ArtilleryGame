#include "buttons.h"
#include "display.h"
#include "stdio.h"

#include "xil_io.h"
#include "xparameters.h"

#define DATA_OFFSET 0x00
#define TRI_OFFSET 0x04
#define PINS_INPUT 0xF
#define PINS_OUTPUT 0x0

// Purpose: Reads the button value status
// Arguments: Offset value
// Return Values: Status of buttons
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_PUSH_BUTTONS_BASEADDR + offset);
}

// Purpose: Writes values to the board
// Arguments: Offset value and value to be written
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_PUSH_BUTTONS_BASEADDR + offset, value);
}

// Initializes the button driver software and hardware.
void buttons_init() { writeRegister(TRI_OFFSET, PINS_INPUT); }

// Returns the current value of all 4 buttons as the lower 4 bits of the
// returned value. bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
uint8_t buttons_read() { return readRegister(DATA_OFFSET); }