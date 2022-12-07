#ifndef TIMER
#define TIMER

#include <stdint.h>
#include <stdbool.h>

void timer_init(float period);

void timer_tick();

uint8_t get_timer_val();

void stop_turn_timer();

bool timer_isexpired();

#endif // TIMER //