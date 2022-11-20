#include "bullet.h"
#include "config.h"

typedef enum { INACTIVE, MOVING, DEAD } bullet_st_t;

// Print the given state passed in by the state variable
static void printStateString(bullet_st_t currentState) {
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

////////// State Machine INIT Functions //////////

// Initialize the missile as a dead missile.  This is useful at the start of the
// game to ensure that player and plane missiles aren't moving before they
// should.
void bullet_init_dead(player_t *player) { player->bullet.currentState = DEAD; }

// Initialize the bullet state machine
void bullet_init(player_t *player, uint16_t x_dest, uint16_t y_dest) {
  // player->type = PLAYER_TYPE_PLAYER; // or ENEMY_TYPE_PLAYER

  player->bullet.x_dest = x_dest;
  player->bullet.y_dest = y_dest;

  player->bullet.x_origin = player->x_current;
  player->bullet.y_origin = player->y_current;
  player->bullet.currentState = INACTIVE;

  //missile->total_length =\
      distance((missile_location_t){missile->x_origin, missile->y_origin},\
               (missile_location_t){missile->x_dest, missile->y_dest});

  player->bullet.x_current = player->bullet.x_origin;
  player->bullet.y_current = player->bullet.y_origin;
}


////////// State Machine TICK Function //////////

// State machine tick function
void bullet_tick(player_t *player) {

}

// Return whether the bullet is dead.
bool bullet_is_dead(player_t *player) { return player->bullet.currentState == DEAD; }

// Return whether the given missile is flying.
//bool bullet_is_flying();

// Used to indicate that the flying bullet should be detonated. This occurs when
// the bullet hits the ground or the player.
void bullet_trigger_explosion(player_t *player) {

}