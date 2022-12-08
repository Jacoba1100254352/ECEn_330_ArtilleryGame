#include "gameControl.h"
#include "bullet.h"
#include "buttons.h"
#include "config.h"
#include "display.h"
#include "displayArtillery.h"
#include "math.h"
#include "playerControl.h"
#include "timer.h"
#include <stdio.h>

bullet_t bullet;

static player_t *currentPlayer;
static player_t *otherPlayer;
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
  printf("Wind val: %d", wind);
  flag_right = (wind > 0);
  displayArtillery_flip_flag(flag_right);
  displayArtillery_update_W_counter_display(abs(wind));
}

static bool checkCollision() {
  if (abs(bullet.x_current - (otherPlayer->x_location + 8)) < 8 &&
      abs(bullet.y_current - (otherPlayer->y_location + 8)) < 8) {
    // do scoring and reset artwork
    currentPlayer->score++;
    printf("Player 1 hit\nPlayer 1 score: %d Player 2 score: %d\n", player1.score, player2.score);
    bullet.dead = true;

    displayArtillery_init();

    displayArtillery_assign_player_location(&player1);
    displayArtillery_assign_player_location(&player2);
  }
}

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  currentPlayer = &player1;
  otherPlayer = &player2;

  displayArtillery_init();
  playerControl_init(&player1, false);
  playerControl_init(&player2, true);
  bullet_init_dead(&bullet);
  timer_init(CONFIG_GAME_TIMER_PERIOD); // Starts the countdown timer

  generateWind();
  displayArtillery_playerDraw(player1_turn, player1, player2);
  displayArtillery_timer_display(30);
  displayArtillery_angle();
  // bullet_init(&bullet, 1, 235, 30, 90 + 45, 0);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() {
  uint8_t buttons = buttons_read();

  timer_tick();
    currentPlayer = (player1_turn) ? &player1 : &player2;
    otherPlayer = (player1_turn) ? &player2 : &player1;

  if (oneshot && buttons & BUTTONS_BTN1_MASK) {
    if (player1_turn) {
      if (player1.changeAngle)
        displayArtillery_power();
      else
        displayArtillery_angle();
      player1.changeAngle = !player1.changeAngle;
    } else if (player2.changeAngle)
      displayArtillery_power();
    else
      displayArtillery_angle();
    player2.changeAngle = !player2.changeAngle;
    oneshot = false;
  } else if (!buttons) // If problem caused, change this
    oneshot = true;

  if ((bullet_is_dead(&bullet) && buttons & BUTTONS_BTN0_MASK) || timer_isexpired()) {
    double angle = (currentPlayer == &player1) ? 90 + player1.angle : -90 - player1.angle;
    bullet_init(&bullet, currentPlayer->x_location, currentPlayer->y_location, currentPlayer->power, angle, wind);
  }

  playerControl_tick(currentPlayer);

  if (!bullet_is_dead(&bullet)) {
    bullet_tick(&bullet);
    oneshot2 = true;
    checkCollision();
  }

  if (bullet_is_dead(&bullet) && oneshot2) {
    // Reset artwork
    displayArtillery_update_B_counter_display(currentPlayer->angle);
    displayArtillery_update_P_counter_display(currentPlayer->power);

    srand((int)bullet.x_vel);
    generateWind();
    timer_init(CONFIG_GAME_TIMER_PERIOD);
    oneshot2 = false;
    player1_turn = !player1_turn;
  }
}