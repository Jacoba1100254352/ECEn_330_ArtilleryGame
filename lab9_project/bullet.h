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
void bullet_init_dead();

// Initialize the bullet state machine. Takes values 30-60 for power and values
// 0-180 for angle (will be tuned)
void bullet_init(uint16_t x_origin, uint16_t y_origin, double power, double angle, double wind);

// State machine tick function
void bullet_tick();

void bullet_init_dead();

// Return whether the bullet is dead.
bool bullet_isDead();

bool bullet_hasCollided(uint16_t player_x, uint16_t player_y, uint16_t player_size);

#endif /* BULLET */