#include "display_artillery.h"
#include "bitmaps.h"
#include "display.h"
#include "time.h"

#define LIGHT_PURPLE_BG_COLOR display_color565(0xEE, 0x54, 0xFA)
#define PURPLE_MOUNTAIN_COLOR display_color565(0x4B, 0x00, 0x72)
#define BOTTOM_LINE_COLOR display_color565(0xA0, 0xA1, 0x39)
#define LINE_COLOR display_color565(0xE6, 0xEA, 0x54)
#define GREEN_NUMBERS_COLOR display_color565(0x03, 0x96, 0x03)
#define SOFT_YELLOW_COLOR display_color565(0xFF, 0xF9, 0xF0)
#define SOFT_GREEN_COLOR display_color565(0xEB, 0xEA, 0x6A)
#define TERRAIN_GREEN_COLOR display_color565(0x90, 0xDF, 0x24)
#define TOP_ROW_DIGITS_Y 7
#define TOP_ROW_LETTERS_Y 20
#define BOTTOM_ROW_DIGITS_Y 217
#define TERRAIN_RECT_WIDTH (DISPLAY_WIDTH / 40)

static uint16_t player_one_ylocation = DISPLAY_HEIGHT;
static uint16_t player_two_ylocation = DISPLAY_HEIGHT;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void display_artillery_init() { // Clear the screen
  srand(time(0));
  display_fillScreen(LIGHT_PURPLE_BG_COLOR);

  // BITMAPINFOHEADER bitmapInfoHeader;
  // uint8_t *bitmap = LoadBitmapFile("Clouds.bmp", &bitmapInfoHeader);
  //  display_drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
  display_drawBitmap(0, 0, line_bitmap, DISPLAY_WIDTH, 3, LINE_COLOR);
  display_drawBitmap(0, 3, top_segment_bitmap, DISPLAY_WIDTH, 30, DISPLAY_BLACK);
  display_drawBitmap(0, 33, line_bitmap, DISPLAY_WIDTH, 3, LINE_COLOR);
  display_drawBitmap(100, 45, clouds_bitmap, 32, 11, SOFT_YELLOW_COLOR);
  display_drawBitmap(100 + 128, 45, clouds_bitmap, 32, 11, SOFT_YELLOW_COLOR);
  display_drawBitmap(0, 64, mountain_top_bitmap, DISPLAY_WIDTH, 16, SOFT_YELLOW_COLOR);
  display_drawBitmap(0, 80, mountain_body_bitmap, DISPLAY_WIDTH, 160, PURPLE_MOUNTAIN_COLOR);

  // Draw Terrain
  for (uint16_t rect = 0; rect < 40; rect++) {
    uint8_t y_placement = rand() % (DISPLAY_HEIGHT / 2) + (DISPLAY_HEIGHT / 2);
    // If x location is between rect positions for player 1
    if ((rect == 4 || rect == 5) && y_placement < player_one_ylocation)
      player_one_ylocation = y_placement;
    else if ((rect == 35 || rect == 36) && y_placement < player_two_ylocation)
      player_two_ylocation = y_placement;
    display_fillRect(TERRAIN_RECT_WIDTH * rect, y_placement, TERRAIN_RECT_WIDTH, DISPLAY_HEIGHT - y_placement,
                     TERRAIN_GREEN_COLOR);
  }

  // B and values
  display_drawBitmap(60, TOP_ROW_LETTERS_Y, B_bitmap, 16, 10, SOFT_GREEN_COLOR);
  display_drawBitmap(80, TOP_ROW_LETTERS_Y, buttons_bitmap, 13, 9, SOFT_YELLOW_COLOR);
  display_drawBitmap(60, TOP_ROW_DIGITS_Y, six_s_bitmap, 16, 10, SOFT_YELLOW_COLOR);
  display_drawBitmap(78, TOP_ROW_DIGITS_Y, zero_s_bitmap, 16, 10, SOFT_YELLOW_COLOR);

  // P and values
  display_drawBitmap(124, TOP_ROW_LETTERS_Y, P_bitmap, 16, 10, SOFT_GREEN_COLOR);
  display_drawBitmap(144, TOP_ROW_LETTERS_Y, buttons_bitmap, 13, 9, SOFT_YELLOW_COLOR);
  display_drawBitmap(124, TOP_ROW_DIGITS_Y, seven_s_bitmap, 16, 10, SOFT_YELLOW_COLOR);
  display_drawBitmap(142, TOP_ROW_DIGITS_Y, zero_s_bitmap, 16, 10, SOFT_YELLOW_COLOR);

  // W and values
  display_drawBitmap(188, TOP_ROW_LETTERS_Y, W_bitmap, 16, 10, SOFT_GREEN_COLOR);
  display_drawBitmap(208, TOP_ROW_LETTERS_Y, flag_bitmap, 13, 11, SOFT_YELLOW_COLOR);
  display_drawBitmap(188, TOP_ROW_DIGITS_Y, one_s_bitmap, 16, 10, SOFT_YELLOW_COLOR);
  display_drawBitmap(206, TOP_ROW_DIGITS_Y, zero_s_bitmap, 16, 10, SOFT_YELLOW_COLOR);

  // Bottom Numbers
  display_drawBitmap(154, BOTTOM_ROW_DIGITS_Y, three_bitmap, 16, 22, GREEN_NUMBERS_COLOR);
  display_drawBitmap(174, BOTTOM_ROW_DIGITS_Y, zero_bitmap, 16, 22, GREEN_NUMBERS_COLOR);
}

void display_artillery_assign_player_location(player_t *player) {
    if (!player->player_num) 
  {
    player->x_location = DISPLAY_WIDTH/8-8;
    player->y_location = player_one_ylocation-16; // minus player height offset
    // display_fillCircle(player->x_location, player->y_location, 10, DISPLAY_WHITE);
    display_drawBitmap(player->x_location, player->y_location, player_left_bitmap, 16, 16, DISPLAY_YELLOW);
  } else {
    player->x_location = DISPLAY_WIDTH * 9 / 10 - 8;
    player->y_location = player_two_ylocation-16; // minus player height offset
    // display_fillCircle(player->x_location, player->y_location, 10, DISPLAY_GREEN);
    display_drawBitmap(player->x_location, player->y_location, player_right_bitmap, 16, 16, DISPLAY_YELLOW);
  }
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.