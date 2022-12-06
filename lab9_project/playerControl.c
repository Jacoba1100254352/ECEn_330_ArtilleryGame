
#include "playerControl.h"
#include "bullet.h"
#include "buttons.h"
#include "config.h"
#include "display.h"

#define WAIT_TIME_ONE_SHOT_S 0.5
#define WAIT_TIME_RAPID_S 0.1

#define WAIT_TIME_ONE_SHOT_TICKS WAIT_TIME_ONE_SHOT_S / CONFIG_GAME_TIMER_PERIOD
#define WAIT_TIME_RAPID_TICKS WAIT_TIME_RAPID_S / CONFIG_GAME_TIMER_PERIOD

typedef enum { INIT, WAIT, SLOW_PRESSED, FAST_PRESSED } playerControl_st_t;
static playerControl_st_t currentState;

void playerControl_init(player_t *player, bool player_num)
{
  player->player_num = player_num;

  if (player_num)
  {
    player->x_location = 50 + (rand() % (DISPLAY_WIDTH / 4));
    player->y_location = DISPLAY_HEIGHT / 4;
    display_drawCircle(player->x_location, player->y_location, 25, DISPLAY_WHITE);
  } else {
    player->x_location = DISPLAY_WIDTH - 50 - (rand() % (DISPLAY_WIDTH / 4));
    player->y_location = DISPLAY_HEIGHT / 4;
    display_drawCircle(player->x_location, player->y_location, 25, DISPLAY_RED);
  }
  player->angle = 90;
  player->power = 45;
}

static void incVal(player_t *player, uint8_t buttons)
{
  if (player->changeAngle)
  {
    player->angle++;
    printf("New angle: %d\n", player->angle);
  } else {
    player->power++;
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
          incVal(player, buttons);
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