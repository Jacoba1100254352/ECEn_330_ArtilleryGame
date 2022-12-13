#ifndef BULLET
#define BULLET

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  double x_vel;
  double y_vel;

  double x_current;
  double y_current;

  double wind;

  bool dead;

} bullet_t;

// Initialize the bullet as dead.
void bullet_init_dead(bullet_t *bullet);

// Initialize the bullet state machine. Takes values 30-60 for power and values
// 0-180 for angle (will be tuned)
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin,
                 double power, double angle, double wind);

// State machine tick function
void bullet_tick(bullet_t *bullet);

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet);

#endif /* BULLET */