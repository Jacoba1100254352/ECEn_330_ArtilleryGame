#include "bullet.h"
#include "config.h"
#include <math.h>

typedef enum { INACTIVE, MOVING, DEAD } bullet_st_t;

double x_vel(uint16_t power, double angle) { return power * sine(angle); }

double y_vel(uint16_t power, double angle) { return power * cos(angle); }

// Print the given state passed in by the state variable
static void
printStateString(bullet_st_t currentState) {
  // Print the given state
  switch (currentState) {
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
static void debug(bullet_st_t currentState) {
  static bullet_st_t previousState = INACTIVE;
  // Print state change when a change occurs
  if (previousState != currentState) {
    printf("\nDEBUG: ");
    printStateString(previousState);
    printf(" -> ");
    printStateString(currentState);

    previousState = currentState;
  }
}

void bullet_init_dead(bullet_t *bullet) { bullet->bullet_is_dead = true; }

// Initialize the bullet state machine
void bullet_init(bullet_t *bullet, uint16_t x_origin, uint16_t y_origin, uint16_t power, double angle) { 
  bullet->x_current = x_origin;
  bullet->y_current = y_origin;


}

////////// State Machine TICK Function //////////

// State machine tick function
void bullet_tick(bullet_t *bullet)
{

}

// Return whether the bullet is dead.
bool bullet_is_dead(bullet_t *bullet)
{

}

// Return whether the given missile is flying.
//bool bullet_is_flying();

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(bullet_t *bullet) { bullet->bullet_splode = true; }