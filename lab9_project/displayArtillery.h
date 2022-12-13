#ifndef DISPLAY_ARTILLERY
#define DISPLAY_ARTILLERY
#include "playerControl.h"

void displayArtillery_init();

void displayArtillery_angle();

void displayArtillery_power();

void displayArtillery_eraseButtons();

void displayArtillery_assign_player_location(player_t *player);

void displayArtillery_draw_top_segment();

void displayArtillery_erase_top_segment();

void displayArtillery_draw_clouds(uint16_t cloud_1_x);

void displayArtillery_timer_display(uint8_t count);
void displayArtillery_timer_erase(uint8_t count);

void displayArtillery_update_B_counter_display(uint8_t count, bool draw);
void displayArtillery_update_P_counter_display(uint8_t count, bool draw);
void displayArtillery_update_W_counter_display(uint8_t count, bool draw);

void displayArtillery_flip_flag(bool direction);

void displayArtillery_playerFlash(bool player1_turn, bool flash);
void displayArtillery_drawPlayers();

void displayArtillery_draw_score(uint8_t score1, uint8_t score2);
void displayArtillery_erase_score(uint8_t score1, uint8_t score2);

#endif /* DISPLAY_ARTILLERY */