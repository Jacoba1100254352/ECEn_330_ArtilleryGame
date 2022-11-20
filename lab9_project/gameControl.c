#include "gameControl.h"
#include "bullet.h"
#include "config.h"
#include "player.h"
#include "touchscreen.h"

// Used for alternating the player
#define PLAYER_OFFSET 1

// If the bullet lands close enough to the player to count (may want to change with addition of radius?)
#define HIT_RANGE 10

player_t players[CONFIG_MAX_PLAYERS];

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void gameControl_init() { // Clear the screen
  display_fillScreen(DISPLAY_BLACK);

  // Initialize the players
  for (uint16_t i = 0; i < CONFIG_MAX_PLAYERS; i++)
    player_init(&players[i]);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() {

  ///   QUESTION!!! Should this be put in the player_tick() function or stay here? What do you think?
  // Initialize Player missiles and shoot if the screen is pressed
  if (touchscreen_get_status() == TOUCHSCREEN_RELEASED) {
    display_point_t coor = touchscreen_get_location();
    for (uint16_t i = 0; i < CONFIG_MAX_PLAYERS; i++)
      if (player_is_turn(&players[i]) && bullet_is_dead(&players[i])) {
        bullet_init(&players[i], coor.x, coor.y);
        break;
      }

    touchscreen_ack_touch();
  }

  // Tick the players
  for (uint16_t i = 0; i < CONFIG_MAX_PLAYERS; i++)
    player_tick(&players[i]);

  // Check if missile i should explode, caused by an exploding missile j
  for (uint8_t i = 0; i < CONFIG_MAX_PLAYERS; i++) {

    // Skip if it is not said players turn (manage the active players bullet) and/or their bullet is dead (not moving)
    if (!player_is_turn(&players[i]) || bullet_is_dead(&players[i]))
      continue;

    uint8_t otherPlayer = abs(i - PLAYER_OFFSET);

    // Determine if one player is within the HIT_RANGE of the other players bullet
    if (abs(players[i].bullet.x_current - players[otherPlayer].x_current) <= HIT_RANGE &&
        abs(players[i].bullet.y_current - players[otherPlayer].y_current) <= HIT_RANGE)
      ; // Player dies (-1 point?)
  }
}