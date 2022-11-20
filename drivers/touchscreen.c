#include "touchscreen.h"
#include "stdio.h"

#define CLEAR 0
#define FIFTYMS 0.05

// Status of the touchscreen
typedef enum { ADC_SETTLING, PRESSED_ST, WAITING } touchscreen_states_t;

static touchscreen_status_t status;
static touchscreen_states_t currentState;
static double adc_timer;
static double adc_settle_ticks;
static display_point_t coor;

// Initialize the touchscreen driver state machine, with a given tick period (in
// seconds).
void touchscreen_init(double period_seconds) {
  adc_timer = CLEAR;
  adc_settle_ticks = FIFTYMS / period_seconds;
  currentState = WAITING;
  display_clearOldTouchData();
  status = TOUCHSCREEN_IDLE;
}

// Tick the touchscreen driver state machine
void touchscreen_tick() {
  // Get the current status of the touchscreen
  bool isTouched = display_isTouched();

  // Transition and Moore output
  switch (currentState) {
  case ADC_SETTLING:
    // Transition to pressed or waiting
    if (!isTouched)
      currentState = WAITING;
    else if (isTouched && adc_timer == adc_settle_ticks) {
      status = TOUCHSCREEN_PRESSED;
      currentState = PRESSED_ST;
      uint8_t z;
      display_getTouchedPoint(&coor.x, &coor.y, &z);
    }
    break;
  case PRESSED_ST:
    if (!isTouched) {
      currentState = WAITING;
      status = TOUCHSCREEN_RELEASED;
    }
    break;
  case WAITING:
    if (isTouched) {
      display_clearOldTouchData();
      currentState = ADC_SETTLING;
    }
    break;
  }

  // Mealy Output
  switch (currentState) {
  case ADC_SETTLING:
    adc_timer++;
    break;
  case WAITING:
    adc_timer = CLEAR;
    break;
  }
}

// Return the current status of the touchscreen
touchscreen_status_t touchscreen_get_status() { return status; }

// Acknowledge the touchscreen touch.  This function will only have effect when
// the touchscreen is in the TOUCHSCREEN_RELEASED status, and will cause it to
// switch to the TOUCHSCREEN_IDLE status.
void touchscreen_ack_touch() {
  if (status == TOUCHSCREEN_RELEASED)
    status = TOUCHSCREEN_IDLE;
}

// Get the (x,y) location of the last touchscreen touch
display_point_t touchscreen_get_location() {
  /*int16_t x, y;
  uint8_t z;
  display_getTouchedPoint(&x, &y, &z);
  display_point_t coor = {x, y};*/
  return coor;
}