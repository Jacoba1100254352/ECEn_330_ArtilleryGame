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
#include <time.h>

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 12
#define PLAYER_SIZE 8

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

static void generateWind() {
  wind = rand() % 10 - 5;
  printf("Wind val: %d", wind);
  flag_right = (wind > 0);
  displayArtillery_flip_flag(flag_right);
  displayArtillery_update_W_counter_display(abs(wind));
}

static bool checkCollision() {
  if (abs(bullet.x_current - (otherPlayer->x_location + PLAYER_SIZE)) < PLAYER_SIZE &&
      abs(bullet.y_current - (otherPlayer->y_location + PLAYER_SIZE)) < PLAYER_SIZE) {
    // do scoring and reset artwork
    currentPlayer->score++;
    printf("Player 1 hit\nPlayer 1 score: %d Player 2 score: %d\n", player1.score, player2.score);
    displayArtillery_score(player1.score, player2.score);
    bullet.dead = true;

    displayArtillery_init();

    displayArtillery_assign_player_location(&player1);
    displayArtillery_assign_player_location(&player2);
  }
}

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  srand(time(0));

  currentPlayer = &player1;
  otherPlayer = &player2;

  oneshot = true;
  oneshot2 = false;
  player1_turn = true;
  flag_right = true;

  wind = 0;

  displayArtillery_init();
  playerControl_init(&player1, false);
  playerControl_init(&player2, true);
  bullet_init_dead(&bullet);
  timer_init(CONFIG_GAME_TIMER_PERIOD); // Starts the countdown timer

  generateWind();
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() {
  static bool triggered = false;
  static uint8_t delay = 0;
  uint8_t buttons = buttons_read();

  timer_tick(player1_turn);

  if (oneshot && buttons & BUTTONS_BTN1_MASK) {
    if (player1_turn) {
      (player1.changeAngle) ? timer_power_isActive() : timer_angle_isActive();
      // Display immediately when changed to avoid confusion
      (!player1.changeAngle) ? displayArtillery_angle() : displayArtillery_power();
      player1.changeAngle = !player1.changeAngle;
    } else {
      (player2.changeAngle) ? timer_power_isActive() : timer_angle_isActive();
      // Display immediately when changed to avoid confusion
      (!player2.changeAngle) ? displayArtillery_angle() : displayArtillery_power();
      player2.changeAngle = !player2.changeAngle;
    }
    oneshot = false;
  } else if (!buttons) // If problem caused, change this
    oneshot = true;

  if ((bullet_is_dead(&bullet) && buttons & BUTTONS_BTN0_MASK) || (timer_isexpired() && !triggered)) {
    double angle = (currentPlayer == &player1) ? 90 + player1.angle : -(90 + player1.angle);
    uint16_t bullet_starting_x = currentPlayer->x_location + ((player1_turn) ? PLAYER_HEIGHT : 0);
    bullet_init(&bullet, bullet_starting_x, currentPlayer->y_location, currentPlayer->power, angle, wind);
    triggered = true;
  }

  playerControl_tick(currentPlayer);

  delay %= 2;
  if (!bullet_is_dead(&bullet) && delay == 1) {
    bullet_tick(&bullet);
    checkCollision();
    oneshot2 = true;
  }
  delay++;

  if (bullet_is_dead(&bullet) && oneshot2) {
    // Reset artwork
    displayArtillery_update_B_counter_display(currentPlayer->angle);
    displayArtillery_update_P_counter_display(currentPlayer->power);

    srand((int)bullet.x_vel);
    generateWind();
    start_turn_timer();

    oneshot2 = false;
    player1_turn = !player1_turn;
    triggered = false;

    // Update the current player for counter display purposes
    currentPlayer = (player1_turn) ? &player1 : &player2;
    otherPlayer = (player1_turn) ? &player2 : &player1;

    // Update counters based on the current players previous settings
    displayArtillery_update_B_counter_display(currentPlayer->angle);
    displayArtillery_update_P_counter_display(currentPlayer->power);

    // Reset players
    displayArtillery_drawPlayers();
  }
}