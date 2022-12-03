#ifndef BULLET
#define BULLET

#include "display.h"
#include "artillery.h"

////////// State Machine INIT Functions //////////

typedef struct {
  int16_t x_vel;
  int16_t y_vel;

  uint16_t x_current;
  uint16_t y_current;

  uint16_t vel_magnitude;

  bool bullet_is_dead;

} bullet_t;

//Initialize the bullet as dead.
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