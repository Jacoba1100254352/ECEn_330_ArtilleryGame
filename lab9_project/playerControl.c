
#include "playerControl.h"
#include "bitmaps.h"
#include "bullet.h"
#include "buttons.h"
#include "config.h"
#include "display.h"
#include "displayArtillery.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DRAW true

#define WAIT_TIME_ONE_SHOT_S 0.5
#define WAIT_TIME_RAPID_S 0.1

#define WAIT_TIME_ONE_SHOT_TICKS WAIT_TIME_ONE_SHOT_S / CONFIG_GAME_TIMER_PERIOD
#define WAIT_TIME_RAPID_TICKS WAIT_TIME_RAPID_S / CONFIG_GAME_TIMER_PERIOD

typedef enum { INIT, WAIT, SLOW_PRESSED, FAST_PRESSED } playerControl_st_t;
static playerControl_st_t currentState;

void playerControl_init(player_t *player, bool player_num) {
  player->player_num = player_num;

  player->changeAngle = true;

  player->angle = 45;
  player->power = 45;

  player->score = 0;

  currentState = INIT;

  displayArtillery_assign_player_location(player);
}

static void incVal(player_t *player, uint8_t buttons) {

  if (player->changeAngle) {
    // Increment or decrement the angle value
    if (buttons & BUTTONS_BTN2_MASK)
      player->angle = fmin(90, player->angle + 1);
    else if (buttons & BUTTONS_BTN3_MASK)
      player->angle = fmax(0, player->angle - 1);
    else
      return;

    // Update the B counter display
    displayArtillery_update_B_counter_display(player->angle, DRAW);
  } else {
    // Increment or decrement the power value
    if (buttons & BUTTONS_BTN2_MASK)
      player->power = fmin(60, player->power + 1);
    else if (buttons & BUTTONS_BTN3_MASK)
      player->power = fmax(30, player->power - 1);
    else
      return;

    // Update the P counter display
    displayArtillery_update_P_counter_display(player->power, DRAW);
  }
}

// Tick the game control logic
//
// This function reads the buttons (with delay) and does the "back door" of
// what will be displayed for the power and angle the player chooses
void playerControl_tick(player_t *player) {
  uint8_t buttons = buttons_read();
  static uint16_t ticks_waited = 0;

  //
  switch (currentState) {
  case (INIT):
    currentState = WAIT;
    break;

  case (WAIT):
    if (buttons) {
      incVal(player, buttons);
      currentState = SLOW_PRESSED;
    }
    break;

  case (SLOW_PRESSED):
    if (buttons && (ticks_waited >= WAIT_TIME_ONE_SHOT_TICKS)) {
      ticks_waited = 0;
      currentState = FAST_PRESSED;
    } else if (!buttons)
      currentState = WAIT;

    ticks_waited++;
    break;

  case (FAST_PRESSED):
    if (buttons && (ticks_waited <= WAIT_TIME_RAPID_TICKS)) {
      incVal(player, buttons);
      ticks_waited = 0;
    } else if (!buttons)
      currentState = WAIT;

    ticks_waited++;
    break;
  }
}