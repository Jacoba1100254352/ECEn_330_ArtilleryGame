#include "gameControl.h"
#include "bullet.h"
#include "config.h"
#include "display.h"

bullet_t bullet;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void gameControl_init() { // Clear the screen
  display_fillScreen(DISPLAY_BLACK);
  bullet_init(&bullet, 100, 100, 5, 1, 0);
  //display_fillScreen(DISPLAY_BLACK);
  //display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);

}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.
void gameControl_tick() { bullet_tick(&bullet); }