#include "gameControl.h"
#include "display_artillery.h"
#include "bullet.h"
#include "config.h"
#include "math.h"
#include "display.h"
#include <stdio.h>

bullet_t bullet;
//const uint8_t *bitmap = Background.bmp;

// Initialize the game control logic
// This function will initialize the screen and players
void gameControl_init() { // Clear the screen
  display_artillery_init();
  bullet_init(&bullet, 1, 235, 30, 90+45, 0);

}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick()
{
  bullet_tick(&bullet);
  if (bullet_is_dead(&bullet))
  {
    printf("I am but a phantom\n");
    //display_artillery_init();
    bullet_init(&bullet, 1, 235, 45, 90+45, 0);
  }
}