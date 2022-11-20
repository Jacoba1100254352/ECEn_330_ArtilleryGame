#ifndef BULLET
#define BULLET

#include "display.h"
#include "artillery.h"

////////// State Machine INIT Functions //////////

// Initialize the missile as a dead missile.  This is useful at the start of the
// game to ensure that player and plane missiles aren't moving before they
// should.
void bullet_init_dead(player_t *player);

// Initialize the bullet state machine
void bullet_init(player_t *player, uint16_t x_dest, uint16_t y_dest);


////////// State Machine TICK Function //////////

// State machine tick function
void bullet_tick(player_t *player);

// Return whether the bullet is dead.
bool bullet_is_dead(player_t *player);

// Return whether the given missile is flying.
//bool bullet_is_flying();

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(player_t *player);

#endif /* BULLET */