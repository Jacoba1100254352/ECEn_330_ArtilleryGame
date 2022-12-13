#ifndef PLAYERCONTROL
#define PLAYERCONTROL

#include "bitmaps.h"
#include <stdbool.h>

typedef struct {
  // The player's x and y locations, to be randomly generated at beginning of each round
  uint16_t x_location;
  uint16_t y_location;

  bool player_num; // 0 (false) player 1, 1 (true) player 2

  uint8_t angle; // the angle chosen by the player to shoot.
  uint8_t power; // the power value displayed. 10x the actual value?

  bool changeAngle; // Alternate between power and angle selection

  uint8_t score; // The players current Score

} player_t;

// Initialize the player control logic
// This function initializes the player, its x/y coordinates, and assigns it a player number
void playerControl_init(player_t *player, bool player_num);

// Tick the game control logic
//
// This function reads the buttons (with delay) and does the "back door" of
// what will be displayed for the power and angle the player chooses
void playerControl_tick(player_t *player);

#endif /* PLAYERCONTROL */