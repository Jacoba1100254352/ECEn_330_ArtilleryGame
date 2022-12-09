#include "timer.h"
#include "displayArtillery.h"

#define ADC_COUNTER_MAX 1
#define AUTO_COUNTER_MAX 10
#define RATE_COUNTER_MAX 1

// States for the controller state machine.
enum timer_st_t {
  init_st,      // Initialize to/start with this state
  waiting_st,   // Wait til turnover
  timer_done_st // Wait until timer reset
};
static enum timer_st_t currentState;

static uint8_t timer;
static uint8_t tick_period;
static bool timer_expired;

void timer_init(float period_seconds) {
  currentState = init_st;
  tick_period = (uint8_t)1/period_seconds;
}

void timer_tick() {
  static uint16_t secCounter = 0;

  // Transition and Moore output
  switch (currentState) {
  case init_st:
    secCounter = 0;
    timer_expired = false;
    timer = 30;
    displayArtillery_timer_display(timer);
    currentState = waiting_st;
    break;
  case waiting_st:
    if (secCounter == tick_period) {
      if (timer == 0) {
        timer_expired = true;
        currentState = timer_done_st;
      } else
        timer--;
      secCounter = 0;
      displayArtillery_timer_display(timer);
    }
    break;
  case timer_done_st:
    break;
  default:
    printf("Error with {currentState:%d}", currentState);
    break;
  }

  // Mealy Output
  switch (currentState) {
  case waiting_st:
    secCounter++;
    break;
  }
}

void stop_turn_timer() { currentState = timer_done_st; }

uint8_t get_timer_val() { return timer; }

bool timer_isexpired() { return timer_expired; }