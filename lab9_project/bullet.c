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

static bullet_t bullet;

double x_vel(double power, double angle) { return power / 10 * sin(angle * M_PI / 180); }
double y_vel(double power, double angle) { return power / 10 * cos(angle * M_PI / 180); }

static bool checkOutOfBounds() {
  return (bullet.x_current > DISPLAY_WIDTH || bullet.x_current < 0 || bullet.y_current > DISPLAY_HEIGHT);
}

// Initialize the bullet state machine
void bullet_init(uint16_t x_origin, uint16_t y_origin, double power, double angle, double wind) {
  bullet.x_current = x_origin;
  bullet.y_current = y_origin;

  displayArtillery_drawBullet(bullet, ERASE);
  bullet.x_vel = x_vel(power, angle);
  bullet.y_vel = y_vel(power, angle);

  bullet.x_current += bullet.x_vel;
  bullet.y_current += bullet.y_vel;
  displayArtillery_drawBullet(bullet, DRAW);

  currentState = INIT;

  bullet.wind = wind / 100;

  bullet.dead = false;
}

// State machine tick function
void bullet_tick() {
  switch (currentState) {
  case INIT:
    currentState = MOVING;
    break;

  case MOVING:
    currentState = (bullet.dead || checkOutOfBounds()) ? DEAD : MOVING;

    displayArtillery_drawBullet(bullet, ERASE);
    bullet.y_vel -= GRAVITY_ACCELERATION;
    bullet.x_vel += bullet.wind;

    bullet.x_current += bullet.x_vel;
    bullet.y_current += bullet.y_vel;
    displayArtillery_drawBullet(bullet, DRAW);
    break;

  case DEAD:
    bullet.dead = true;
    break;
  }
}

void bullet_init_dead() { bullet.dead = true; }

// Return whether the bullet is dead.
bool bullet_isDead() { return bullet.dead; }

bool bullet_hasCollided(uint16_t player_x, uint16_t player_y, uint16_t player_size) {
  return (abs(bullet.x_current - (player_x + player_size)) < player_size &&
          abs(bullet.y_current - (player_y + player_size)) < player_size);
}