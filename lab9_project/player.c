#include "player.h"

enum player_st_t {
INACTIVE, MOVING, DEAD
};
static enum player_st_t currentState;

// Print the given state passed in by the state variable
static void printStateString(enum player_st_t state) {
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
static void debug() {
  static enum player_st_t previousState = INACTIVE;
  // Print state change when a change occurs
  if (previousState != currentState) {
    printf("\nDEBUG: ");
    printStateString(previousState);
    printf(" -> ");
    printStateString(currentState);

    previousState = currentState;
  }
}


// Initialize the player state machine
void player_init(player_t *player) {
  
  bullet_init_dead(&player);

}

// State machine tick function
void player_tick(player_t *player) {

}

// Return whether it's the given players turn
bool player_is_turn(player_t *player) { return player->is_turn; }