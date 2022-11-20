/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

#ifndef ARMINTERRUPTS
#define ARMINTERRUPTS

#include <stdbool.h>
#include <stdint.h>

#include "xil_types.h"

//#include "../src/laserTag/queue.h"

// These values can be used with interrupts_getAdcInputMode() to determine the
// current mode for the ADC input.

// Positive input voltage 0V:1V -> 0:4095
#define INTERRUPTS_ADC_UNIPOLAR_MODE true

// Bipolar input voltage -0.5V:+0.5V -> -2048:+2047 (two's complement).
#define INTERRUPTS_ADC_BIPOLAR_MODE false

// Change the default here.
#define INTERRUPTS_ADC_DEFAULT_INPUT_MODE INTERRUPTS_ADC_UNIPOLAR_MODE

// The ZYBO board routes for inputs of the XADC to the XADC PMOD.
// Set SELECTED_XADC_CHANNEL to the desired channel.
// Channel 14 is the channel that is used for the laser-tag system.

// pins JA3 (P) and JA9 (N) on the ZYBO board.
#define XADC_AUX_CHANNEL_15 XSM_CH_AUX_MAX

// pins JA1 (P) and JA7 (N) on the ZYBO board.
#define XADC_AUX_CHANNEL_14 XSM_CH_AUX_MAX - 1

// pins JA2 (P) and JA8 (N) on the ZYBO board.
#define XADC_AUX_CHANNEL_7 XSM_CH_AUX_MAX - 8

// pins JA4 (P) and JA10 (N) on the ZYBO board.
#define XADC_AUX_CHANNEL_6 XSM_CH_AUX_MAX - 9

// This line selects the correct ADC input channel for the 330 baseboard.
#define SELECTED_XADC_CHANNEL XADC_AUX_CHANNEL_14

// This line selects some other ADC input channel for the 330 baseboard.
//#define SELECTED_XADC_CHANNEL XADC_AUX_CHANNEL_15

// Uses interval timer 0 to measure time spent in ISR.
#define ENABLE_INTERVAL_TIMER_0_IN_TIMER_ISR 1
// Interval timer to use to capture total ISR runtime
#define INTERRUPT_CUMULATIVE_ISR_INTERVAL_TIMER_NUMBER 0

#ifdef __cplusplus
extern "C" {
#endif

// Initialize ARM interrupts
int armInterrupts_init();

// Global interrupt enable/disable
void armInterrupts_enable();
void armInterrupts_disable();

// Register different interrupt handlers
int32_t armInterrupts_setupTimer(void (*isr)(), double period_seconds);
void armInterrupts_enableTimer();
void armInterrupts_disableTimer();

int32_t armInterrupts_setupIntc(void (*isr)());
void armInterrupts_enableIntc();
void armInterrupts_disableIntc();

uint32_t armInterrupts_getTimerIsrCount();

// **********************************************************
// These have not been tested since major changes
// **********************************************************

// Globally enable/disable SysMon interrupts.
int armInterrupts_enableSysMonGlobalInts();
int armInterrupts_disableSysMonGlobalInts();

// Enable End-Of-Conversion interrupts. You can use this to count how often an
// ADC conversion occurs.
int armInterrupts_enableSysMonEocInts();
int armInterrupts_disableSysMonEocInts();

// Used to determine the input mode for the ADC.
bool armInterrupts_getAdcInputMode();

// Use this to read the latest ADC conversion.
uint32_t armInterrupts_getAdcData();

// u32 interrupts_getTotalXadcSampleCount();
u32 armInterrupts_getTotalEocCount();

// Init/Enable/disable interrupts for the bluetooth radio (RDYN line).
uint32_t armInterrupts_initBluetoothInterrupts();
void armInterrupts_enableBluetoothInterrupts();
void armInterrupts_disableBluetoothInterrupts();
void armInterrupts_ackBluetoothInterrupts();

#ifdef __cplusplus
} // extern "C'
#endif

#endif /* ARMINTERRUPTS */
