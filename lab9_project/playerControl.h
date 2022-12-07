#ifndef PLAYERCONTROL
#define PLAYERCONTROL

#include "bitmaps.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint16_t x_location; //The player's x and y locations, to be randomly generated at beginning of round
  uint16_t y_location;

  bool player_num; //0 (false) player 1, 1 (true) player 2

  int16_t angle; //the angle chosen by the player to shoot.

  uint16_t power; //the power value displayed. 10x the actual value?

  bool changeAngle; //

  uint8_t score;
} player_t;

// Initialize the player control logic
// This function initializes the player, its x/y coordinates, and assigns it which player it is.

// NOTE FOR OTHER TEAM MEMBERS: I assume we will create two players in gameControl and run it from there
void playerControl_init(player_t *player, bool player_num);

// Tick the game control logic
//
// This function reads the buttons (with delay) and does the "back door" of
// what will be displayed for the power and angle the player chooses
void playerControl_tick(player_t *player);

//Returns the current selected power.
//Can be used with display to display the power. Returns 10x the actual power
uint16_t playerPower(player_t *player);

//Returns the current selected angle in degrees.
//Can be used with display to display angle.
uint16_t playerAngle(player_t *player);

#endif /* PLAYERCONTROL */