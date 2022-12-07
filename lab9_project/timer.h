#ifndef TIMER
#define TIMER

#include <stdint.h>

void timer_init(float period);

void timer_tick();

uint8_t get_timer_val();

#endif // TIMER //