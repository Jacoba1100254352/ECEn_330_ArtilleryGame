#include "gameControl.h"
#include "display_artillery.h"
#include "bullet.h"
#include "config.h"

bullet_t bullet;
//const uint8_t *bitmap = Background.bmp;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void gameControl_init() { // Clear the screen
  display_artillery_init();
  bullet_init(&bullet, 100, 100, 5, 1, 0);

}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() { bullet_tick(&bullet); }