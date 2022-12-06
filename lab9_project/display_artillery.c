#include "display_artillery.h"

#include "display.h"

#define CLOUD_COLOR display_color565(0xFA, 0xDC, 0x5E)
#define MOUNTAIN_BODY_COLOR display_color565(0x3E, 0x00, 0x71)
#define MOUNTAIN_TOP_COLOR display_color565(0xF4, 0xE0, 0x77)
#define BOTTOM_LINE_COLOR display_color565(0xA0, 0xA1, 0x39)
#define TOP_LINE_COLOR display_color565(0xE6, 0xEA, 0x54)
#define NUMBERS_COLOR display_color565(0x36, 0x58, 0x21)

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void display_artillery_init() { // Clear the screen
  display_fillScreen(DISPLAY_DARK_RED);

  // BITMAPINFOHEADER bitmapInfoHeader;
  // uint8_t *bitmap = LoadBitmapFile("Clouds.bmp", &bitmapInfoHeader);
  //  display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
  display_drawBitmap(0, 0, line_bitmap, DISPLAY_WIDTH, 3, TOP_LINE_COLOR);
  display_drawBitmap(0, 3, top_segment_bitmap, DISPLAY_WIDTH, 30, DISPLAY_BLACK);
  display_drawBitmap(0, 33, line_bitmap, DISPLAY_WIDTH, 3, TOP_LINE_COLOR);
  display_drawBitmap(100, 45, clouds_bitmap, 32, 11, CLOUD_COLOR);
  display_drawBitmap(0, 64, mountain_top_bitmap, DISPLAY_WIDTH, 16, MOUNTAIN_TOP_COLOR);
  display_drawBitmap(0, 80, mountain_body_bitmap, DISPLAY_WIDTH, 160, MOUNTAIN_BODY_COLOR);
  display_drawBitmap(0, 80, zero_bitmap, 16, 22, NUMBERS_COLOR);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.