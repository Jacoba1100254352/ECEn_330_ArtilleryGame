#ifndef PLAYERCONTROL
#define PLAYERCONTROL

#include "display.h"

typedef struct {
  uint16_t x_location; //The player's x and y locations, to be randomly generated at beginning of round
  uint16_t y_location;

  bool player_turn; //0 (false) player 1, 1 (true) player 2

  int16_t angle; //the angle chosen by the player to shoot.

  double power; //The actual value passed to the bullet function.

  uint16_t power; //the power value displayed. 10x the actual value?
} player_t;

// Initialize the player control logic
// This function initializes the player, its x/y coordinates, and assigns it which player it is.

// NOTE FOR OTHER TEAM MEMBERS: I assume we will use a player array with two players in it (a la missilecommand)
void playerControl_init(player_t *player);

// Tick the game control logic
//
// This function reads the buttons (with delay) and does the "back door" of
// what will be displayed for the power and angle the player chooses
void playerControl_tick();

#endif /* GAMECONTROL */