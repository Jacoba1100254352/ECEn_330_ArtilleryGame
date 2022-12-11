#include "bullet.h"
#include "config.h"
#include "display.h"
#include "timer.h"
#include "displayArtillery.h"
#include <math.h>
#include <stdio.h>

#define BULLET_WIDTH 5
#define BULLET_HEIGHT 3

typedef enum { INIT, INACTIVE, MOVING, DEAD } bullet_st_t;
static bullet_st_t currentState;

double x_vel(double power, double angle) { return power / 10 * sin(angle * M_PI / ((double)180)); }

double y_vel(double power, double angle) { return power / 10 * cos(angle * M_PI / ((double)180)); }

static void drawBullet(bullet_t *bullet, bool erase) {
  int16_t color = (erase) ? DISPLAY_RED : DISPLAY_WHITE;
  display_drawBitmap(bullet->x_current, bullet->y_current, bullet_bitmap, BULLET_WIDTH, BULLET_HEIGHT, color);
  //display_fillRect(bullet->x_current, bullet->y_current, 3, 3, color);
  //printf("%d\n", display_readPixel(0, 0));
}

static bool checkOutOfBounds(bullet_t *bullet) {
  return (bullet->x_current > DISPLAY_WIDTH || bullet->x_current < 0 || bullet->y_current > DISPLAY_HEIGHT);
}

void bullet_init_dead(bullet_t *bullet) { bullet->dead = true; }

// Initialize the bullet state machine
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin, double power, double angle, double wind) {
  bullet->x_current = x_origin;
  bullet->y_current = y_origin;

  drawBullet(bullet, true);
  bullet->x_vel = x_vel(power, angle);
  bullet->y_vel = y_vel(power, angle);

  bullet->x_current += bullet->x_vel;
  bullet->y_current += bullet->y_vel;
  drawBullet(bullet, false);

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
    stop_turn_timer(); // Stop the timer once you are moving
    currentState = (bullet->dead || checkOutOfBounds(bullet)) ? DEAD : MOVING;

    drawBullet(bullet, true);
    bullet->y_vel -= CONFIG_GRAVITY_ACCELERATION;
    bullet->x_vel += bullet->wind;

    bullet->x_current += bullet->x_vel;
    bullet->y_current += bullet->y_vel;
    drawBullet(bullet, false);
    break;

  case DEAD:
    bullet->dead = true;
    break;

  default:
    printf("UNKNOWN_ST");
    break;
  }
}

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet) { return bullet->dead; }

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(bullet_t *bullet) { bullet->splode = true; }