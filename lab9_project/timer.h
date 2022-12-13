#ifndef TIMER
#define TIMER

#include <stdbool.h>

void timer_init(float period);
void timer_tick(bool player1_turn);

void timer_start_turn_timer();
void timer_stop_turn_timer();

bool timer_isExpired();

void timer_power_setActive();
void timer_angle_setActive();

void timer_hide_button(bool hide);

void timer_start_score_timer();
bool timer_score_timer_done();

#endif // TIMER //