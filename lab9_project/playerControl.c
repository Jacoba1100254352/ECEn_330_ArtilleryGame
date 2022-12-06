
#include "playerControl.h"
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


void playerControl_init(player_t *player, bool player_num)
{
  player->player_num = player_num;

  if (!player_num) 
  {
    player->x_location = 25 + (rand() % (DISPLAY_WIDTH / 6));
    player->y_location = DISPLAY_HEIGHT - DISPLAY_HEIGHT / 4;
    //display_fillCircle(player->x_location, player->y_location, 10, DISPLAY_WHITE);
    display_drawBitmap(0, 80, player_left_bitmap, 16, 16, DISPLAY_YELLOW);
  } else {
    player->x_location = DISPLAY_WIDTH - 25 - (rand() % (DISPLAY_WIDTH / 6));
    player->y_location = DISPLAY_HEIGHT - DISPLAY_HEIGHT / 4;
    //display_fillCircle(player->x_location, player->y_location, 10, DISPLAY_GREEN);
    display_drawBitmap(0, 80, player_right_bitmap, 16, 16, DISPLAY_YELLOW);
  }
  player->angle = 90;
  player->power = 45;

  player->changeAngle = true;
}

static void incVal(player_t *player, uint8_t buttons)
{

  if (player->changeAngle)
  {
    if (buttons & BUTTONS_BTN2_MASK)
    {
      if (player->angle < 180 && player->angle > 0)
        player->angle++;
      }
    else if (buttons & BUTTONS_BTN3_MASK)
    {
      if (player->angle <= 180 && player->angle > 0)
        player->angle--;
    }
    printf("New angle: %d\n", player->angle);
  } else {
    if (buttons & BUTTONS_BTN2_MASK)
    {
      if (player->power < 60 && player->power > 30)
        player->power++;
    }
    else if (buttons & BUTTONS_BTN3_MASK)
    {
      if (player->power < 60 && player->power > 30)
        player->power--;
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