
#include "playerControl.h"
#include "bullet.h"
#include "buttons.h"

void playerControl_init(player_t *player, bool player_num)
{
    //Assign player number

    //Randomly generate the x and y of each player given which player it is


}

// Tick the game control logic
//
// This function reads the buttons (with delay) and does the "back door" of
// what will be displayed for the power and angle the player chooses
void playerControl_tick(player_t *player);

//Returns the current selected power.
//Can be used with display to display the power. Returns 10x the actual power
uint16_t playerPower(player_t *player) { return player->power; }

//Returns the current selected angle in degrees.
//Can be used with display to display angle.
uint16_t playerAngle(player_t *player) { return player->angle; }