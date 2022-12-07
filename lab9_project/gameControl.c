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
static bool oneshot2 = false;
static bool player1_turn = true;
static bool flag_right = true;

static int8_t wind = 0;
// const uint8_t *bitmap = Background.bmp;

static void generateWind() {
  wind = -5 + rand() % 10;
  if (wind > 0)
    flag_right = true;
  else
    flag_right = false;
  display_artillery_flip_flag(flag_right);
  display_artillery_update_W_counter_display(abs(wind));
}

static bool checkCollision()
{
  if (player1_turn)
  {
    if (abs(bullet.x_current - (player2.x_location+8)) < 8 && abs(bullet.y_current - (player2.y_location+8)) < 8)
    {
      // do scoring and reset artwork
      player1.score++;
      printf("Player 1 hit\nPlayer 1 score: %d Player 2 score: %d\n", player1.score, player2.score);
      bullet.dead = true;
    }
  }
  else
  {
    if ((abs(bullet.x_current - (player1.x_location+8)) < 8) && (abs(bullet.y_current - (player1.y_location+8)) < 8))
    {
       //do scoring
      player2.score++;
      printf("Player 2 hit\nPlayer 1 score: %d Player 2 score: %d\n", player1.score, player2.score);
      bullet.dead = true;
    }
  }
}

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  buttons_init();
  display_artillery_init();
  playerControl_init(&player1, false);
  playerControl_init(&player2, true);
  bullet_init_dead(&bullet);
  generateWind();

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
      bullet_init(&bullet, player1.x_location, player1.y_location, player1.power, 90 + player1.angle, 0);
    else
      bullet_init(&bullet, player2.x_location, player2.y_location, player2.power, -90 - player1.angle, 0);
    //player1_turn = !player1_turn;
  }
  if (player1_turn)
    playerControl_tick(&player1);
  else
    playerControl_tick(&player2);
  if (!bullet_is_dead(&bullet))
  {
    bullet_tick(&bullet);
    oneshot2 = true;
    checkCollision();
  }

  if (bullet_is_dead(&bullet) && oneshot2)
  {
    //Reset artwork
    generateWind();
    oneshot2 = false;
    player1_turn = !player1_turn;
  }
}