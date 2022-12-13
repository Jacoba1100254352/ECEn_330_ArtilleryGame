#include "gameControl.h"
#include "config.h"
#include "bullet.h"
#include "buttons.h"
#include "displayArtillery.h"
#include "playerControl.h"
#include "timer.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DRAW true
#define HIDE_BUTTON true
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 12
#define PLAYER_SIZE 8

bullet_t bullet;

static player_t *currentPlayer;
static player_t *otherPlayer;
player_t player1;
player_t player2;

static bool button_isReleased;
static bool bullet_wasFired;
static bool player1_turn;
static bool score_isDisplayed;

static int8_t wind = 0;

static void generateWind() {
  wind = rand() % 10 - 5;
  bool flag_right = (wind > 0);
  displayArtillery_flip_flag(flag_right);
  displayArtillery_update_W_counter_display(abs(wind), DRAW);
}

static void checkCollision() {
  if (!(abs(bullet.x_current - (otherPlayer->x_location + PLAYER_SIZE)) < PLAYER_SIZE &&
        abs(bullet.y_current - (otherPlayer->y_location + PLAYER_SIZE)) < PLAYER_SIZE))
    return;

  // Update and display score, stop bullet
  currentPlayer->score++;
  bullet.dead = true;
  score_isDisplayed = true;

  // Clear the timer and display the score
  timer_stop_turn_timer();
  displayArtillery_draw_score(player1.score, player2.score);
  timer_start_score_timer();
}

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  srand(time(0));

  currentPlayer = &player1;
  otherPlayer = &player2;

  button_isReleased = true;
  bullet_wasFired = false;
  player1_turn = true;
  score_isDisplayed = false;

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
  static uint8_t delay = 1;
  uint8_t buttons = buttons_read();

  timer_tick(player1_turn);

  if (score_isDisplayed)
    if (timer_score_timer_done()) {
      // Erase the scores
      displayArtillery_erase_score(player1.score, player2.score);
      score_isDisplayed = false;

      // Reset the screen
      displayArtillery_init();

      displayArtillery_assign_player_location(&player1);
      displayArtillery_assign_player_location(&player2);
    } else
      return;

  if (button_isReleased && buttons & BUTTONS_BTN1_MASK) {
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
    button_isReleased = false;
  } else if (!buttons) // If no buttons are pressed, allow a button switch again
    button_isReleased = true;

  // When button fire is initially triggered
  if ((bullet_is_dead(&bullet) && buttons & BUTTONS_BTN0_MASK) || (timer_isexpired() && !triggered)) {
    // Stop the timer once you are moving and erase the top segment (stop flashing too)
    timer_stop_turn_timer();
    timer_hide_button(HIDE_BUTTON);
    displayArtillery_erase_top_segment();

    double angle = (currentPlayer == &player1) ? 90 + player1.angle : -(90 + player1.angle);
    uint16_t bullet_starting_x = currentPlayer->x_location + ((player1_turn) ? PLAYER_HEIGHT : 0);
    bullet_init(&bullet, bullet_starting_x, currentPlayer->y_location, currentPlayer->power, angle, wind);
    triggered = true;
  }

  playerControl_tick(currentPlayer);

  // Reset, the bullet has stopped moving
  if (bullet_is_dead(&bullet) && bullet_wasFired) {
    // Reset artwork
    timer_hide_button(!HIDE_BUTTON);
    displayArtillery_draw_top_segment();

    displayArtillery_update_B_counter_display(currentPlayer->angle, DRAW);
    displayArtillery_update_P_counter_display(currentPlayer->power, DRAW);

    srand((int)bullet.x_vel);
    generateWind();
    timer_start_turn_timer();

    bullet_wasFired = false;
    player1_turn = !player1_turn;
    triggered = false;

    // Update the current player for counter display purposes
    currentPlayer = (player1_turn) ? &player1 : &player2;
    otherPlayer = (player1_turn) ? &player2 : &player1;

    // Update counters based on the current players previous settings
    displayArtillery_update_B_counter_display(currentPlayer->angle, DRAW);
    displayArtillery_update_P_counter_display(currentPlayer->power, DRAW);

    // Reset players
    displayArtillery_drawPlayers();
  } else if (!bullet_is_dead(&bullet) && delay % 2 == 1) { // Firing the bullet with the possibility of a delay
    bullet_tick(&bullet);
    checkCollision();
    bullet_wasFired = true;
  }
  // delay++; // uncomment to make the bullet appear slightly slower (and more like the original game)
}