#ifndef BULLET
#define BULLET

#include "display.h"
#include "artillery.h"

////////// State Machine INIT Functions //////////

typedef struct {
  double x_vel;
  double y_vel;

  uint16_t x_current;
  uint16_t y_current;

  int16_t wind;

  bool dead;

  bool splode;

} bullet_t;

//Initialize the bullet as dead.
void bullet_init_dead(bullet_t *bullet);

// Initialize the bullet state machine
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin, uint16_t power, double angle, int16_t wind);


////////// State Machine TICK Function //////////

// State machine tick function
void bullet_tick(bullet_t *bullet);

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet);

// Return whether the given missile is flying.
//bool bullet_is_flying();

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(bullet_t *bullet);

#endif /* BULLET */