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
static bool player1_turn = true;
static bool flag_right = true;

static int8_t wind = 0;
// const uint8_t *bitmap = Background.bmp;

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  buttons_init();
  display_artillery_init();
  playerControl_init(&player1, false);
  playerControl_init(&player2, true);
  bullet_init_dead(&bullet);
  wind = -5 + rand() % 10;
  if (wind > 0)
    flag_right = true;
  else
    flag_right = false;
  display_artillery_flip_flag(wind);
  display_artillery_update_W_counter_display(abs(wind));
  // bullet_init(&bullet, 1, 235, 30, 90 + 45, 0);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick()
{
  //static uint8_t count = 0;
  uint8_t buttons = buttons_read();

  //display_artillery_update_B_counter_display(count);
  //display_artillery_update_P_counter_display(count);
  //display_artillery_update_W_counter_display(count);
  //count++;

  if (oneshot && buttons & BUTTONS_BTN1_MASK) {
    if (player1_turn)
      player1.changeAngle = !player1.changeAngle;
    else
      player2.changeAngle = !player2.changeAngle;
    oneshot = false;
  }
  if (!buttons)
    oneshot = true;
  if (bullet_is_dead(&bullet) && buttons & BUTTONS_BTN0_MASK)
  {
    if(player1_turn)
      bullet_init(&bullet, player1.x_location, player1.y_location, player1.power, 90 + player1.angle, -5);
    else
      bullet_init(&bullet, player2.x_location, player2.y_location, player2.power, -90 - player1.angle, -5);
    player1_turn = !player1_turn;
  }
  if (player1_turn)
    playerControl_tick(&player1);
  else
    playerControl_tick(&player2);
  if (!bullet_is_dead(&bullet))
    bullet_tick(&bullet);
}