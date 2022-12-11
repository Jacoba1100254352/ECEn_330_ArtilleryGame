#ifndef TIMER
#define TIMER

#include "playerControl.h"
#include <stdbool.h>
#include <stdint.h>

void timer_init(float period);
void timer_tick(bool player1_turn);

void start_turn_timer();
void stop_turn_timer();

bool timer_isexpired();

void timer_power_isActive();
void timer_angle_isActive();

#endif // TIMER //