#include "gameControl.h"
#include "bullet.h"
#include "buttons.h"
#include "config.h"
#include "displayArtillery.h"
#include "playerControl.h"
#include "timer.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define DRAW true
#define HIDE_BUTTON true
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 12
#define PLAYER_SIZE 8

#define CLOUD_DELAY_STRENGTH 3
#define MAX_WIND_RANGE 10
#define MIN_WIND_STRENGTH -5

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
  wind = rand() % MAX_WIND_RANGE + MIN_WIND_STRENGTH;
  bool flag_right = (wind > 0);
  displayArtillery_flip_flag(flag_right);
  displayArtillery_update_W_counter_display(abs(wind), DRAW);
}

static void checkCollision() {
  if (!bullet_hasCollided(otherPlayer->x_location, otherPlayer->y_location, PLAYER_SIZE))
    return;

  // Update and display score, stop bullet
  currentPlayer->score++;
  bullet_init_dead();
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

  bullet_init_dead();
  displayArtillery_init();
  playerControl_init(&player1, false);
  playerControl_init(&player2, true);
  timer_init(CONFIG_GAME_TIMER_PERIOD); // Starts the countdown timer

  generateWind();
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() {
  static bool triggered = false;
  static uint8_t bulletDelay = 1;
  static uint8_t cloudDelay = 1;
  uint8_t buttons = buttons_read();

  timer_tick(player1_turn);
  playerControl_tick(currentPlayer);

  if (cloudDelay % CLOUD_DELAY_STRENGTH == 1)
    displayArtillery_cloud_tick();

  // If displaying score, tick score display timer then reset
  if (score_isDisplayed)
    if (timer_score_timer_done()) {
      // Erase the scores
      displayArtillery_erase_score(player1.score, player2.score);
      score_isDisplayed = false;

      // Reset the screen
      displayArtillery_init();

      displayArtillery_assign_player_location(&player1);
      displayArtillery_assign_player_location(&player2);
    } else // Don't allow anything else to happen/tick until timer is up
      return;

  // When button fire is initially triggered
  if ((bullet_isDead() && !bullet_wasFired && button_isReleased && buttons & BUTTONS_BTN0_MASK) ||
      (timer_isExpired() && !triggered)) {
    // Stop the timer once you are moving and erase the top segment (stop flashing too)
    timer_stop_turn_timer();
    timer_hide_button(HIDE_BUTTON);
    displayArtillery_erase_top_segment();

    double angle = (currentPlayer == &player1) ? 90 + player1.angle : -(90 + player2.angle);
    uint16_t bullet_starting_x = currentPlayer->x_location + ((player1_turn) ? PLAYER_HEIGHT : 0);
    bullet_init(bullet_starting_x, currentPlayer->y_location, currentPlayer->power, angle, wind);

    triggered = true;
    button_isReleased = false;
  } else if (button_isReleased && buttons & BUTTONS_BTN1_MASK) { // If BTN1 is pressed then alternate between angle and power
    // Determine active button and display immediately when changed to avoid confusion
    if (!currentPlayer->changeAngle) {
      timer_angle_setActive();
      displayArtillery_angle();
    } else {
      timer_power_setActive();
      displayArtillery_power();
    }

    // Change active button
    currentPlayer->changeAngle = !currentPlayer->changeAngle;

    button_isReleased = false;
  } else if (!buttons)
    button_isReleased = true;


  // Reset, the bullet has stopped moving
  if (bullet_isDead() && bullet_wasFired) {
    // Reset artwork
    timer_hide_button(!HIDE_BUTTON);
    displayArtillery_draw_top_segment();

    // srand((int)bullet.x_vel);
    generateWind();
    timer_start_turn_timer();

    // Initialize booleans
    bullet_wasFired = false;
    triggered = false;

    // Switch and update the current player
    player1_turn = !player1_turn;
    currentPlayer = (player1_turn) ? &player1 : &player2;
    otherPlayer = (player1_turn) ? &player2 : &player1;

    // Update counters based on the current players previous settings
    displayArtillery_update_B_counter_display(currentPlayer->angle, DRAW);
    displayArtillery_update_P_counter_display(currentPlayer->power, DRAW);

    // Make sure the correct button is being displayed
    if (currentPlayer->changeAngle) {
      timer_angle_setActive();
      displayArtillery_angle();
    } else {
      timer_power_setActive();
      displayArtillery_power();
    }

    // Reset players
    displayArtillery_drawPlayers();
  } else if (!bullet_isDead() && bulletDelay % 2 == 1) { // Firing the bullet with the possibility of a delay
    bullet_tick();
    checkCollision();
    bullet_wasFired = true;
  }
  // bulletDelay++; // uncomment to make the bullet appear slightly slower (and more like the original game)
  cloudDelay++;
}