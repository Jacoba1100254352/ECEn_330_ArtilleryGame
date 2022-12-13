#ifndef TIMER
#define TIMER

#include "playerControl.h"
#include <stdbool.h>
#include <stdint.h>

void timer_init(float period);
void timer_tick(bool player1_turn);

void timer_start_turn_timer();
void timer_stop_turn_timer();

bool timer_isexpired();

void timer_power_isActive();
void timer_angle_isActive();

void timer_hide_button(bool hide);

void timer_start_score_timer();
bool timer_score_timer_done();

#endif // TIMER //