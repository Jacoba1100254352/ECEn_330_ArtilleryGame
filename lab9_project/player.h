#ifndef PLAYER
#define PLAYER

#include "display.h"
#include "artillery.h"

// Initialize the player state machine
void player_init(player_t *player);

// State machine tick function
void player_tick(player_t *player);

// Return whether it's the given players turn
bool player_is_turn(player_t *player);

#endif /* PLAYER */