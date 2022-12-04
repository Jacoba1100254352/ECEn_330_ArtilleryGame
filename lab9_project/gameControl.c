#include "gameControl.h"
#include "bullet.h"
#include "config.h"
#include "touchscreen.h"

bullet_t bullet;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void gameControl_init() { // Clear the screen
  display_fillScreen(DISPLAY_BLUE);
  bullet_init(&bullet, 40, 280, 15, 1.5, 0);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() { bullet_tick(&bullet); }