#include "bullet.h"
#include "config.h"
#include "display.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>

typedef enum { INIT, INACTIVE, MOVING, DEAD } bullet_st_t;
static bullet_st_t currentState;

double x_vel(double power, double angle) {
  return power / 10 * sin(angle * M_PI / ((double)180));
}

double y_vel(double power, double angle) {
  return power / 10 * cos(angle * M_PI / ((double)180));
}

static void drawBullet(bullet_t *bullet, bool erase) {
  int16_t color;
  if (erase)
    color = DISPLAY_RED;
  else
    color = DISPLAY_WHITE;

  display_fillRect(bullet->x_current, bullet->y_current, 3, 3, color);
}

static bool checkOutOfBounds(bullet_t *bullet) {
  if (bullet->x_current > DISPLAY_WIDTH || bullet->x_current < 0)
    return true;
  else if (bullet->y_current > DISPLAY_HEIGHT)
    return true;
  else
    return false;
}

// Print the given state passed in by the state variable
static void printStateString() {
  // Print the given state
  switch (currentState) {
  case INIT:
    printf("INIT");
    break;
  case INACTIVE:
    printf("INACTIVE");
    break;
  case MOVING:
    printf("MOVING");
    break;
  case DEAD:
    printf("DEAD");
    break;
  default:
    printf("UNKNOWN_ST");
    break;
  }
}

// Debug function for printing state changes
static void debug() {
  static bullet_st_t previousState = INIT;
  // Print state change when a change occurs
  if (previousState != currentState) {
    printf("\nDEBUG: ");
    printStateString(previousState);
    printf(" -> ");
    printStateString(currentState);

    previousState = currentState;
  }
}

void bullet_init_dead(bullet_t *bullet) { bullet->dead = true; }

// Initialize the bullet state machine
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin,
                 double power, double angle, double wind) {
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
  // static double power = 0;
  switch (currentState) {
  case INIT:
    // printf("INIT");
    currentState = MOVING;
    break;
  case MOVING:
    stop_turn_timer(); // Stop the timer once you are moving
    if (bullet->dead || checkOutOfBounds(bullet))
      currentState = DEAD;
    else
      currentState = MOVING;

    drawBullet(bullet, true);
    bullet->y_vel -= CONFIG_GRAVITY_ACCELERATION;
    bullet->x_vel += bullet->wind;

    bullet->x_current += bullet->x_vel;
    bullet->y_current += bullet->y_vel;
    drawBullet(bullet, false);

    // printf("x: %f y: %f\n", bullet->x_vel, bullet->y_vel);
    break;
  case DEAD:
    bullet->dead = true;
    // bullet_init(bullet, DISPLAY_WIDTH / 4, 100, power, 90+45, 0);
    break;
  default:
    printf("UNKNOWN_ST");
    break;
  }
}

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet) { return bullet->dead; }

// Return whether the given missile is flying.
// bool bullet_is_flying();

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(bullet_t *bullet) { bullet->splode = true; }