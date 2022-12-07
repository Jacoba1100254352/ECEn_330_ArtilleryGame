#ifndef BULLET
#define BULLET

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

////////// State Machine INIT Functions //////////

typedef struct {
  double x_vel;
  double y_vel;

  double x_current;
  double y_current;

  double wind;

  bool dead;

  bool splode;

} bullet_t;

//Initialize the bullet as dead.
void bullet_init_dead(bullet_t *bullet);

// Initialize the bullet state machine. Takes values 30-60 for power and values 0-180 for angle (will be tuned)
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin, double power, double angle, double wind);


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