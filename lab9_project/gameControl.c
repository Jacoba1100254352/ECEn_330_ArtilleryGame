#include "gameControl.h"
#include "playerControl.h"
#include "display_artillery.h"
#include "bullet.h"
#include "config.h"
#include "math.h"
#include "display.h"
#include "buttons.h"
#include <stdio.h>

bullet_t bullet;

player_t player1;
player_t player2;

static bool oneshot = true;
// const uint8_t *bitmap = Background.bmp;

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  buttons_init();
  display_artillery_init();
  playerControl_init(&player1, false);
  playerControl_init(&player2, true);
  bullet_init_dead(&bullet);
  //bullet_init(&bullet, 1, 235, 30, 90 + 45, 0);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick()
{
  uint8_t buttons = buttons_read();

  if (oneshot && buttons & BUTTONS_BTN1_MASK)
  {
    player1.changeAngle = !player1.changeAngle;
    oneshot = false;
  }
  if (!buttons)
    oneshot = true;

  if (bullet_is_dead(&bullet) && buttons & BUTTONS_BTN0_MASK)
    bullet_init(&bullet, player1.x_location, player1.y_location, player1.power, player1.angle, 0);

  playerControl_tick(&player1);
  if(!bullet_is_dead(&bullet))
    bullet_tick(&bullet);
}