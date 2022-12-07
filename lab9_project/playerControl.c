
#include "playerControl.h"
#include "display_artillery.h"
#include "bullet.h"
#include "buttons.h"
#include "config.h"
#include "display.h"
#include "bitmaps.h"
#include <stdio.h>
#include <stdlib.h>

#define WAIT_TIME_ONE_SHOT_S 0.5
#define WAIT_TIME_RAPID_S 0.1

#define WAIT_TIME_ONE_SHOT_TICKS WAIT_TIME_ONE_SHOT_S / CONFIG_GAME_TIMER_PERIOD
#define WAIT_TIME_RAPID_TICKS WAIT_TIME_RAPID_S / CONFIG_GAME_TIMER_PERIOD

typedef enum { INIT, WAIT, SLOW_PRESSED, FAST_PRESSED } playerControl_st_t;
static playerControl_st_t currentState;
static uint8_t timer;

void playerControl_init(player_t *player, bool player_num)
{
  player->player_num = player_num;
  display_artillery_assign_player_location(player);
  

  player->angle = 45;
  player->power = 45;

  player->changeAngle = true;

  player->score = 0;
}

static void incVal(player_t *player, uint8_t buttons)
{

  if (player->changeAngle)
  {
    if (buttons & BUTTONS_BTN2_MASK) {
      if (player->angle < 90 && player->angle >= 0)
        player->angle++;
      display_artillery_update_B_counter_display(player->angle);
    } else if (buttons & BUTTONS_BTN3_MASK) {
      if (player->angle <= 90 && player->angle > 0)
        player->angle--;
      display_artillery_update_B_counter_display(player->angle);
    }
    //display_artillery_update_B_counter_display(player->angle);
    printf("New angle: %d\n", player->angle);
  } else {
    if (buttons & BUTTONS_BTN2_MASK)
    {
      if (player->power < 60 && player->power >= 30)
        player->power++;
      display_artillery_update_P_counter_display(player->power);
    }
    else if (buttons & BUTTONS_BTN3_MASK)
    {
      if (player->power <= 60 && player->power > 30)
        player->power--;
      display_artillery_update_P_counter_display(player->power);
    }
    printf("New power: %d\n", player->power);
  }
}

// Tick the game control logic
//
// This function reads the buttons (with delay) and does the "back door" of
// what will be displayed for the power and angle the player chooses
void playerControl_tick(player_t *player) { 
    uint8_t buttons = buttons_read();
    static uint16_t ticks_waited = 0;
    switch (currentState) {
    case (INIT):
      currentState = WAIT;
      break;
    case (WAIT):
        if (buttons)
        {
          incVal(player, buttons);
          currentState = SLOW_PRESSED;
        }
        else
          currentState = WAIT;
        break;
    case (SLOW_PRESSED):
        if(buttons && (ticks_waited >= WAIT_TIME_ONE_SHOT_TICKS)){
          ticks_waited = 0;
          currentState = FAST_PRESSED;
          }
        else if (!buttons)
          currentState = WAIT;
        else
          currentState = SLOW_PRESSED;
          ticks_waited++;
          break;
    case (FAST_PRESSED):
        if (buttons && (ticks_waited <= WAIT_TIME_RAPID_TICKS))
        {
          incVal(player, buttons);
          ticks_waited = 0;
        } 
        else if (!buttons)
          currentState = WAIT;
        ticks_waited++;
        break;
    }
}

//Returns the current selected power.
//Can be used with display to display the power. Returns 10x the actual power
uint16_t playerPower(player_t *player) { return player->power; }

//Returns the current selected angle in degrees.
//Can be used with display to display angle.
uint16_t playerAngle(player_t *player) { return player->angle; }