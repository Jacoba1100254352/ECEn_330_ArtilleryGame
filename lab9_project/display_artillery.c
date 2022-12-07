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
#define PLAYER_DIMENSION 16
#define TOP_ROW_DIGITS_Y 7
#define TOP_ROW_LETTERS_Y 20
#define BOTTOM_DIGITS_Y 217
#define BOTTOM_DIGIT_1_X 154
#define BOTTOM_DIGIT_2_X 174
#define BOTTOM_DIGIT_HEIGHT 22
#define FLAG_HEIGHT 11
#define BUTTONS_HEIGHT 9
#define SIDE_IMG_WIDTH 13
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 10

#define TERRAIN_RECT_WIDTH (DISPLAY_WIDTH / 40)

static uint8_t player_one_ylocation;
static uint8_t player_two_ylocation;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void display_artillery_init() { // Clear the screen
  display_fillScreen(LIGHT_PURPLE_BG_COLOR);

  player_one_ylocation = DISPLAY_HEIGHT;
  player_two_ylocation = DISPLAY_HEIGHT;

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

  // Randomly Generate Terrain
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t y_placement[40];
    for (uint8_t rectIndex = 0; rectIndex < 40; rectIndex++) {
      y_placement[rectIndex] = rand() % (DISPLAY_HEIGHT / 3) - (DISPLAY_HEIGHT / 2);
      display_fillRect(TERRAIN_RECT_WIDTH * rectIndex, y_placement[rectIndex], TERRAIN_RECT_WIDTH,
                       DISPLAY_HEIGHT - y_placement[rectIndex], TERRAIN_GREEN_COLOR);
    }
    for (uint8_t rectIndex = 0; rectIndex < 40; rectIndex++) {
      display_fillRect(TERRAIN_RECT_WIDTH * rectIndex, y_placement[rectIndex], TERRAIN_RECT_WIDTH,
                       DISPLAY_HEIGHT - y_placement[rectIndex], PURPLE_MOUNTAIN_COLOR);
    }
  }

  // Draw Final Terrain
  for (uint8_t rectIndex = 0; rectIndex < 40; rectIndex++) {
    uint8_t y_placement = rand() % (DISPLAY_HEIGHT / 3) - (DISPLAY_HEIGHT / 2);
    // If x location is between rect positions for player 1
    if ((rectIndex == 4 || rectIndex == 5) && y_placement < player_one_ylocation)
      player_one_ylocation = y_placement;
    else if ((rectIndex == 35 || rectIndex == 36) && y_placement < player_two_ylocation)
      player_two_ylocation = y_placement;
    display_fillRect(TERRAIN_RECT_WIDTH * rectIndex, y_placement, TERRAIN_RECT_WIDTH, DISPLAY_HEIGHT - y_placement,
                     TERRAIN_GREEN_COLOR);
  }

  // B and values
  display_drawBitmap(60, TOP_ROW_LETTERS_Y, B_bitmap, CHAR_WIDTH, CHAR_HEIGHT, SOFT_GREEN_COLOR);
  display_drawBitmap(80, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
  display_artillery_update_B_counter_display(45);

  // P and values
  display_drawBitmap(124, TOP_ROW_LETTERS_Y, P_bitmap, CHAR_WIDTH, CHAR_HEIGHT, SOFT_GREEN_COLOR);
  display_drawBitmap(144, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
  display_artillery_update_P_counter_display(45);

  // W and values
  display_drawBitmap(188, TOP_ROW_LETTERS_Y, W_bitmap, CHAR_WIDTH, CHAR_HEIGHT, SOFT_GREEN_COLOR);
  //display_drawBitmap(208, TOP_ROW_LETTERS_Y, flag_R_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, SOFT_YELLOW_COLOR);
  display_artillery_update_W_counter_display(0);


  // Bottom Numbers
  //display_artillery_timer_display(30);
}

void display_artillery_angle()
{
  display_drawBitmap(80, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, SOFT_YELLOW_COLOR);
  display_drawBitmap(144, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
}

void display_artillery_power()
{
  display_drawBitmap(80, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(144, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, SOFT_YELLOW_COLOR);
}

void display_artillery_assign_player_location(player_t *player) {
  if (!player->player_num) {
    player->x_location = DISPLAY_WIDTH / 8 - 8;
    player->y_location = player_one_ylocation - PLAYER_DIMENSION; // minus player height offset
    display_drawBitmap(player->x_location, player->y_location, player_left_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       DISPLAY_YELLOW);
  } else {
    player->x_location = DISPLAY_WIDTH * 9 / 10 - 8;
    player->y_location = player_two_ylocation - PLAYER_DIMENSION; // minus player height offset
    display_drawBitmap(player->x_location, player->y_location, player_right_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       DISPLAY_YELLOW);
  }
}

void display_artillery_timer_display(uint8_t count) {
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;
  display_drawBitmap(BOTTOM_DIGIT_1_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[tensPlace]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     GREEN_NUMBERS_COLOR);
  display_drawBitmap(BOTTOM_DIGIT_2_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[onesPlace]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     GREEN_NUMBERS_COLOR);
}

void display_artillery_update_B_counter_display(uint8_t count) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

  // Clear previous value
  display_drawBitmap(60, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevTensPlace]), CHAR_WIDTH, CHAR_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(78, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevOnesPlace]), CHAR_WIDTH, CHAR_HEIGHT, DISPLAY_BLACK);

  // Draw current value
  display_drawBitmap(60, TOP_ROW_DIGITS_Y, &(*counterBitmaps[tensPlace]), CHAR_WIDTH, CHAR_HEIGHT, SOFT_YELLOW_COLOR);
  display_drawBitmap(78, TOP_ROW_DIGITS_Y, &(*counterBitmaps[onesPlace]), CHAR_WIDTH, CHAR_HEIGHT, SOFT_YELLOW_COLOR);

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}

void display_artillery_update_P_counter_display(uint8_t count) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

  // Clear previous value
  display_drawBitmap(124, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevTensPlace]), CHAR_WIDTH, CHAR_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(142, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevOnesPlace]), CHAR_WIDTH, CHAR_HEIGHT, DISPLAY_BLACK);

  // Draw current value
  display_drawBitmap(124, TOP_ROW_DIGITS_Y, &(*counterBitmaps[tensPlace]), CHAR_WIDTH, CHAR_HEIGHT, SOFT_YELLOW_COLOR);
  display_drawBitmap(142, TOP_ROW_DIGITS_Y, &(*counterBitmaps[onesPlace]), CHAR_WIDTH, CHAR_HEIGHT, SOFT_YELLOW_COLOR);

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}

void display_artillery_update_W_counter_display(uint8_t count) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

  // Clear previous value
  display_drawBitmap(188, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevTensPlace]), CHAR_WIDTH, CHAR_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(206, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevOnesPlace]), CHAR_WIDTH, CHAR_HEIGHT, DISPLAY_BLACK);

  // Draw current value
  display_drawBitmap(188, TOP_ROW_DIGITS_Y, &(*counterBitmaps[tensPlace]), CHAR_WIDTH, CHAR_HEIGHT, SOFT_YELLOW_COLOR);
  display_drawBitmap(206, TOP_ROW_DIGITS_Y, &(*counterBitmaps[onesPlace]), CHAR_WIDTH, CHAR_HEIGHT, SOFT_YELLOW_COLOR);

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}

void display_artillery_flip_flag(bool direction) {
  if (!direction)
  {
    display_drawBitmap(208, TOP_ROW_LETTERS_Y, flag_R_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, DISPLAY_BLACK);
    display_drawBitmap(208, TOP_ROW_LETTERS_Y, flag_L_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, SOFT_YELLOW_COLOR);
  }
  else
  {
    display_drawBitmap(208, TOP_ROW_LETTERS_Y, flag_L_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, DISPLAY_BLACK);
    display_drawBitmap(208, TOP_ROW_LETTERS_Y, flag_R_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, SOFT_YELLOW_COLOR);
  }
}

void display_player_1(player_t *player)
{
  display_drawBitmap(player->x_location, player->y_location, player_left_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       SOFT_YELLOW_COLOR);
}

void display_player_2(player_t *player)
{
  display_drawBitmap(player->x_location, player->y_location, player_right_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       SOFT_YELLOW_COLOR);
}

void display_player_1_turn(player_t *player)
{
  display_drawBitmap(player->x_location, player->y_location, player_left_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       DISPLAY_BLACK);
  display_drawBitmap(player->x_location, player->y_location, player_left_turn_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       SOFT_YELLOW_COLOR);
}

void display_player_2_turn(player_t *player)
{
  display_drawBitmap(player->x_location, player->y_location, player_right_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       DISPLAY_BLACK);
  display_drawBitmap(player->x_location, player->y_location, player_right_turn_bitmap, PLAYER_DIMENSION, PLAYER_DIMENSION,
                       SOFT_YELLOW_COLOR);
}

// Tick the game control logic
//
// This function should tick the missiles, handle screen touches, collisions,
// and updating statistics.