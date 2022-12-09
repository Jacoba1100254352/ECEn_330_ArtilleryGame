#ifndef DISPLAY_ARTILLERY
#define DISPLAY_ARTILLERY
#include "playerControl.h"

void displayArtillery_init();

void displayArtillery_angle();

void displayArtillery_power();

void displayArtillery_assign_player_location(player_t *player);

void displayArtillery_timer_display(uint8_t count);

void displayArtillery_update_B_counter_display(uint8_t count);

void displayArtillery_update_P_counter_display(uint8_t count);

void displayArtillery_update_W_counter_display(uint8_t count);

void displayArtillery_flip_flag(bool direction);

void displayArtillery_playerDraw(bool player1_turn, player_t player1,
                                 player_t player2);

#endif /* DISPLAY_ARTILLERY */