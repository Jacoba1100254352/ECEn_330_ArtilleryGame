#include "bullet.h"
#include "display.h"
#include "displayArtillery.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>

#define GRAVITY_ACCELERATION -.1

#define ERASE true
#define DRAW false

typedef enum { INIT, INACTIVE, MOVING, DEAD } bullet_st_t;
static bullet_st_t currentState;

double x_vel(double power, double angle) { return power / 10 * sin(angle * M_PI / ((double)180)); }

double y_vel(double power, double angle) { return power / 10 * cos(angle * M_PI / ((double)180)); }

static bool checkOutOfBounds(bullet_t *bullet) {
  return (bullet->x_current > DISPLAY_WIDTH || bullet->x_current < 0 || bullet->y_current > DISPLAY_HEIGHT);
}

void bullet_init_dead(bullet_t *bullet) { bullet->dead = true; }

// Initialize the bullet state machine
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin, double power, double angle, double wind) {
  bullet->x_current = x_origin;
  bullet->y_current = y_origin;

  displayArtillery_drawBullet(bullet, ERASE);
  bullet->x_vel = x_vel(power, angle);
  bullet->y_vel = y_vel(power, angle);

  bullet->x_current += bullet->x_vel;
  bullet->y_current += bullet->y_vel;
  displayArtillery_drawBullet(bullet, DRAW);

  currentState = INIT;

  bullet->wind = wind / 100;

  bullet->dead = false;
}

////////// State Machine TICK Function //////////

// State machine tick function
void bullet_tick(bullet_t *bullet) {
  switch (currentState) {
  case INIT:
    currentState = MOVING;
    break;

  case MOVING:
    currentState = (bullet->dead || checkOutOfBounds(bullet)) ? DEAD : MOVING;

    displayArtillery_drawBullet(bullet, ERASE);
    bullet->y_vel -= GRAVITY_ACCELERATION;
    bullet->x_vel += bullet->wind;

    bullet->x_current += bullet->x_vel;
    bullet->y_current += bullet->y_vel;
    displayArtillery_drawBullet(bullet, DRAW);
    break;

  case DEAD:
    bullet->dead = true;
    break;
  }
}

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet) { return bullet->dead; }