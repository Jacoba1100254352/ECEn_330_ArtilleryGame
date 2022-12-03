#include "bullet.h"
#include "config.h"
#include <math.h>

typedef enum {INIT, INACTIVE, MOVING, DEAD } bullet_st_t;
bullet_st_t currentState;

double x_vel(uint16_t power, double angle) { return power * sine(angle); }

double y_vel(uint16_t power, double angle) { return power * cos(angle); }

// Print the given state passed in by the state variable
static void
printStateString() {
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
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin, uint16_t power, double angle, int16_t wind) {
  bullet->x_current = x_origin;
  bullet->y_current = y_origin;

  bullet->x_vel = x_vel(power, angle);
  bullet->y_vel = y_vel(power, angle);

  currentState = INIT;

  bullet->wind = wind;
}

////////// State Machine TICK Function //////////

// State machine tick function
void bullet_tick(bullet_t *bullet)
{
  switch (currentState) {
  case INIT:
    currentState = INACTIVE;
    break;
  case INACTIVE:

    
    break;
  case MOVING:
    if (bullet->dead)
      currentState = DEAD;
    else
      currentState = MOVING;

    bullet->y_vel += CONFIG_GRAVITY_ACCELERATION;
    bullet->x_vel += bullet->wind;
    break;
  case DEAD:
    printf("DEAD");
    break;
  default:
    printf("UNKNOWN_ST");
    break;
  }
}

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet) { return bullet->dead; }

// Return whether the given missile is flying.
//bool bullet_is_flying();

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(bullet_t *bullet) { bullet->splode = true; }