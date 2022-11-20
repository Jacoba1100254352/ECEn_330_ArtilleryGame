#include "interrupts.h"
#include "armInterrupts.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdio.h>

#define IPR 0x04
#define IER 0x08
#define IAR 0x0C
#define SIE 0x10
#define CIE 0x14
#define MER 0x1C

#define ISR_PTR_SIZE 3
#define ENABLE 1

#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

// Purpose: Reads the switch value status
// Arguments: Offset value
// Return Values: Status of switches
static uint32_t readRegister(uint32_t offset) {
  // Switch addresses based on timer number
  return Xil_In32(XPAR_AXI_INTC_0_BASEADDR + offset);
}

// Purpose: Writes values to the board
// Arguments: Offset value and value to be written
static void writeRegister(uint32_t offset, uint32_t value) {
  // Switch addresses based on timer number
  Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + offset, value);
}

// Function for array of function pointers
static void (*isrFcnPtrs[ISR_PTR_SIZE])() = {NULL};

// Purpose: Detect and manage the interrupt
static void interrupts_isr() {
  // Loop through each interrupt input
  for (uint8_t i = 0; i < ISR_PTR_SIZE; i++)
    // Check if it has an interrupt pending
    if (readRegister(IPR) & (1 << i)) {
      // Check if there is a callback
      if (isrFcnPtrs[i])
        // Call the callback function
        isrFcnPtrs[i]();
      writeRegister(IAR, 1 << i);
    }
}

// Initialize interrupt hardware
// This function should:
// 1. Configure AXI INTC registers to:
//  - Enable interrupt output (see Master Enable Register)
//  - Disable all interrupt input lines.
// 2. Enable the Interrupt system on the ARM processor, and register an ISR
// handler function. This is done by calling:
//  - armInterrupts_init()
//  - armInterrupts_setupIntc(isr_fcn_ptr)
//  - armInterrupts_enable()
void interrupts_init() {
  writeRegister(MER, ENABLE | (ENABLE << 1));
  interrupts_irq_disable(TIMER_0);
  interrupts_irq_disable(TIMER_1);
  interrupts_irq_disable(TIMER_2);
  armInterrupts_init();
  armInterrupts_setupIntc(interrupts_isr);
  armInterrupts_enable();
}

// Register a callback function (fcn is a function pointer to this callback
// function) for a given interrupt input number (irq).  When this interrupt
// input is active, fcn will be called.
void interrupts_register(uint8_t irq, void (*fcn)()) { isrFcnPtrs[irq] = fcn; }

// Enable single input interrupt line, given by irq number.
void interrupts_irq_enable(uint8_t irq) { writeRegister(SIE, 1 << irq); }

// Disable single input interrupt line, given by irq number.
void interrupts_irq_disable(uint8_t irq) { writeRegister(CIE, 1 << irq); }