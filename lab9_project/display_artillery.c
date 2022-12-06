#include "display_artillery.h"
#include "bitmaps.h"
#include "display.h"

#define CLOUD_COLOR display_color565(0xFA, 0xDC, 0x5E)
#define MOUNTAIN_BODY_COLOR display_color565(0x3E, 0x00, 0x71)
#define MOUNTAIN_TOP_COLOR display_color565(0xF4, 0xE0, 0x77)
#define BOTTOM_LINE_COLOR display_color565(0xA0, 0xA1, 0x39)
#define TOP_LINE_COLOR display_color565(0xE6, 0xEA, 0x54)

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void display_artillery_init() { // Clear the screen
  display_fillScreen(DISPLAY_DARK_RED);

  // BITMAPINFOHEADER bitmapInfoHeader;
  // uint8_t *bitmap = LoadBitmapFile("Clouds.bmp", &bitmapInfoHeader);
  //  display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
  display_drawBitmap(0, 0, clouds_bitmap, 96, 20, CLOUD_COLOR);
  display_drawBitmap(0, 80, mountain_body_bitmap, DISPLAY_WIDTH, 160, MOUNTAIN_BODY_COLOR);
  display_drawBitmap(0, 56, mountain_top_bitmap, DISPLAY_WIDTH, 24, MOUNTAIN_TOP_COLOR);
  //display_drawBitmap(0, 0, top_line_bitmap, DISPLAY_WIDTH, 5, TOP_LINE_COLOR);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.