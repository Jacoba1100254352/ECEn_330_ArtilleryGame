#ifndef DISPLAY_ARTILLERY
#define DISPLAY_ARTILLERY

#include "bullet.h"
#include "playerControl.h"

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void displayArtillery_init();

// Other Display Functions

void displayArtillery_cloud_tick();

void displayArtillery_draw_clouds(int16_t cloud_set_x);
void displayArtillery_erase_clouds(int16_t cloud_set_x);

void displayArtillery_drawBullet(bullet_t bullet, bool erase);

void displayArtillery_timer_display(uint8_t count);
void displayArtillery_timer_erase(uint8_t count);

// Top Display Functions

void displayArtillery_angle();
void displayArtillery_power();

void displayArtillery_update_B_counter_display(uint8_t count, bool draw);
void displayArtillery_update_P_counter_display(uint8_t count, bool draw);
void displayArtillery_update_W_counter_display(uint8_t count, bool draw);

void displayArtillery_flip_flag(bool direction);

void displayArtillery_eraseButtons();

void displayArtillery_draw_top_segment();
void displayArtillery_erase_top_segment();

// Player Display Functions

void displayArtillery_playerFlash(bool player1_turn, bool flash);
void displayArtillery_assign_player_location(player_t *player);
void displayArtillery_drawPlayers();

// Score Functions

void displayArtillery_draw_score(uint8_t score1, uint8_t score2);
void displayArtillery_erase_score(uint8_t score1, uint8_t score2);

#endif /* DISPLAY_ARTILLERY */