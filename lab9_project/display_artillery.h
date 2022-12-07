#ifndef DISPLAY_ARTILLERY
#define DISPLAY_ARTILLERY
#include "playerControl.h"

void display_artillery_init();

void display_artillery_assign_player_location(player_t *player);

void display_artillery_timer_display(uint8_t count);

void display_artillery_update_B_counter_display(uint8_t count);

void display_artillery_update_P_counter_display(uint8_t count);

void display_artillery_update_W_counter_display(uint8_t count);

void display_artillery_flip_flag(bool direction);

void display_player_1(player_t *player)

void display_player_2(player_t *player)

void display_player_1_turn(player_t *player);

void display_player_2_turn(player_t *player);

#endif /* DISPLAY_ARTILLERY */