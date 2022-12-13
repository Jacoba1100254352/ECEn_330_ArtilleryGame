#include "displayArtillery.h"
#include "bitmaps.h"
#include "display.h"
#include "time.h"

// Colors
#define BOTTOM_LINE_COLOR display_color565(0xA0, 0xA1, 0x39)
#define GREEN_NUMBERS_COLOR display_color565(0x03, 0x96, 0x03)
#define LINE_COLOR display_color565(0xE6, 0xEA, 0x54)
#define LIGHT_PURPLE_BG_COLOR display_color565(0xEE, 0x54, 0xFA)
#define PURPLE_MOUNTAIN_COLOR display_color565(0x4B, 0x00, 0x72)
#define SOFT_GREEN_COLOR display_color565(0xEB, 0xEA, 0x6A)
#define SOFT_YELLOW_COLOR display_color565(0xFF, 0xF9, 0xF0)
#define TERRAIN_GREEN_COLOR display_color565(0x90, 0xDF, 0x24)

// X Locations
#define ANGLE_BUTTON_X 80
#define B_DIGIT_1_X 60
#define B_DIGIT_2_X 78
#define BOTTOM_DIGIT_1_X 154
#define BOTTOM_DIGIT_2_X 174
#define CLOUD_X 100
#define CLOUD_X_SEPARATION 128
#define FLAG_X 208
#define P_DIGIT_1_X 124
#define P_DIGIT_2_X 142
#define PLAYER_1_X DISPLAY_WIDTH / 8 - 6
#define PLAYER_2_X DISPLAY_WIDTH * 9 / 10 - 12
#define POWER_BUTTON_X 144
#define SCORE_1_X 96
#define SCORE_2_X 222
#define SCREEN_LEFT_EDGE 0
#define SCREEN_TOP_EDGE 0
#define TURN_INDICATOR_1_X PLAYER_1_X + 2
#define TURN_INDICATOR_2_X PLAYER_2_X + 8
#define W_DIGIT_1_X 188
#define W_DIGIT_2_X 206

// Y Locations
#define BOTTOM_DIGITS_Y 217
#define CLOUD_Y 45
#define LINE_2_Y (LINE_HEIGHT + TOP_SEGMENT_HEIGHT)
#define MOUNTAIN_BODY_Y 80
#define MOUNTAIN_TOP_Y 64
#define TOP_ROW_DIGITS_Y 7
#define TOP_ROW_LETTERS_Y 20
#define TOP_SEGMENT_Y LINE_HEIGHT

// Heights
#define BOTTOM_DIGIT_HEIGHT 22
#define BUTTONS_HEIGHT 9
#define CHAR_HEIGHT 10
#define CLOUD_HEIGHT 11
#define FLAG_HEIGHT 11
#define INDICATOR_HEIGHT 2
#define LINE_HEIGHT 3
#define MOUNTAIN_BODY_HEIGHT 160 // DISPLAY_HEIGHT - MOUNTAIN_BODY_Y
#define MOUNTAIN_TOP_HEIGHT 16
#define PLAYER_HEIGHT 12
#define TOP_SEGMENT_HEIGHT 30

// Widths
#define CHAR_WIDTH 16
#define CLOUD_WIDTH 32
#define INDICATOR_WIDTH 6
#define PLAYER_WIDTH 16
#define SIDE_IMG_WIDTH 13
#define TERRAIN_RECT_WIDTH (DISPLAY_WIDTH / NUM_TERRAIN_RECTANGLES)

// Initial Values
#define INITIAL_COUNTER_VALUE 45
#define INITIAL_TIMER_VALUE 30
#define INITIAL_WIND_SPEED 0

// Other
#define DISP_SOLID_PLAYER false
#define DRAW true
#define HEIGHT_RANGE (DISPLAY_HEIGHT / 3)
#define MAX_HEIGHT (DISPLAY_HEIGHT / 2)
#define NUM_TERRAIN_GENERATIONS 10
#define NUM_TERRAIN_RECTANGLES 40
#define FLAG_ARRAY_SIZE 22
#define INDICATOR_OFFSET 4

static uint8_t player_one_ylocation;
static uint8_t player_two_ylocation;

// Initialize the game control logic
// This function will initialize all missiles, stats, plane, etc.
void displayArtillery_init() {
  // Draw background color
  display_fillScreen(LIGHT_PURPLE_BG_COLOR);

  player_one_ylocation = DISPLAY_HEIGHT;
  player_two_ylocation = DISPLAY_HEIGHT;

  // Draw background

  display_drawBitmap(SCREEN_LEFT_EDGE, MOUNTAIN_TOP_Y, mountain_top_bitmap, DISPLAY_WIDTH, MOUNTAIN_TOP_HEIGHT,
                     SOFT_YELLOW_COLOR);
  display_drawBitmap(SCREEN_LEFT_EDGE, MOUNTAIN_BODY_Y, mountain_body_bitmap, DISPLAY_WIDTH, MOUNTAIN_BODY_HEIGHT,
                     PURPLE_MOUNTAIN_COLOR);

  // Randomly Generate Terrain
  for (uint8_t i = 0; i < NUM_TERRAIN_GENERATIONS; i++) {
    uint8_t y_placement[NUM_TERRAIN_RECTANGLES];
    for (uint8_t rectIndex = 0; rectIndex < NUM_TERRAIN_RECTANGLES; rectIndex++) {
      y_placement[rectIndex] = rand() % HEIGHT_RANGE - MAX_HEIGHT; // Center on the bottom of the screen
      display_fillRect(TERRAIN_RECT_WIDTH * rectIndex, y_placement[rectIndex], TERRAIN_RECT_WIDTH,
                       DISPLAY_HEIGHT - y_placement[rectIndex], TERRAIN_GREEN_COLOR);
    }
    for (uint8_t rectIndex = 0; rectIndex < NUM_TERRAIN_RECTANGLES; rectIndex++)
      display_fillRect(TERRAIN_RECT_WIDTH * rectIndex, y_placement[rectIndex], TERRAIN_RECT_WIDTH,
                       DISPLAY_HEIGHT - y_placement[rectIndex], PURPLE_MOUNTAIN_COLOR);
  }

  // Draw Final Terrain
  for (uint8_t rectIndex = 0; rectIndex < NUM_TERRAIN_RECTANGLES; rectIndex++) {
    uint8_t y_placement = rand() % HEIGHT_RANGE - MAX_HEIGHT; // Center on the bottom of the screen
    // If x location is between rect positions for player 1
    if ((rectIndex == 4 || rectIndex == 5) && y_placement < player_one_ylocation)
      player_one_ylocation = y_placement - PLAYER_HEIGHT;
    else if ((rectIndex == 35 || rectIndex == 36) && y_placement < player_two_ylocation)
      player_two_ylocation = y_placement - PLAYER_HEIGHT;
    display_fillRect(TERRAIN_RECT_WIDTH * rectIndex, y_placement, TERRAIN_RECT_WIDTH, DISPLAY_HEIGHT - y_placement,
                     TERRAIN_GREEN_COLOR);
  }

  displayArtillery_draw_top_segment();

  // Bottom Numbers
  displayArtillery_timer_display(INITIAL_TIMER_VALUE);

  // Initialize to start with the angle being displayed
  displayArtillery_angle();

  // Display Players
  displayArtillery_drawPlayers();
}
// Flash buttons every 0.25 seconds
// Erase top row and timer when firing
void displayArtillery_angle() {
  display_drawBitmap(ANGLE_BUTTON_X, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT,
                     SOFT_YELLOW_COLOR);
  display_drawBitmap(POWER_BUTTON_X, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
}

void displayArtillery_power() {
  display_drawBitmap(ANGLE_BUTTON_X, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(POWER_BUTTON_X, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT,
                     SOFT_YELLOW_COLOR);
}

void displayArtillery_eraseButtons() {
  display_drawBitmap(ANGLE_BUTTON_X, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(POWER_BUTTON_X, TOP_ROW_LETTERS_Y, buttons_bitmap, SIDE_IMG_WIDTH, BUTTONS_HEIGHT, DISPLAY_BLACK);
}

void displayArtillery_assign_player_location(player_t *player) {
  if (!player->player_num) {
    player->x_location = PLAYER_1_X;
    player->y_location = player_one_ylocation; // minus player height offset
    display_drawBitmap(player->x_location, player->y_location, player_left_bitmap, PLAYER_WIDTH, PLAYER_HEIGHT,
                       DISPLAY_YELLOW);
  } else {
    player->x_location = PLAYER_2_X;
    player->y_location = player_two_ylocation; // minus player height offset
    display_drawBitmap(player->x_location, player->y_location, player_right_bitmap, PLAYER_WIDTH, PLAYER_HEIGHT,
                       DISPLAY_YELLOW);
  }
}

void displayArtillery_draw_top_segment() {
  display_drawBitmap(SCREEN_LEFT_EDGE, SCREEN_TOP_EDGE, line_bitmap, DISPLAY_WIDTH, LINE_HEIGHT, LINE_COLOR);
  display_drawBitmap(SCREEN_LEFT_EDGE, TOP_SEGMENT_Y, top_segment_bitmap, DISPLAY_WIDTH, TOP_SEGMENT_HEIGHT,
                     DISPLAY_BLACK);
  display_drawBitmap(SCREEN_LEFT_EDGE, LINE_2_Y, line_bitmap, DISPLAY_WIDTH, LINE_HEIGHT, LINE_COLOR);

  // B and values
  display_drawBitmap(B_DIGIT_1_X, TOP_ROW_LETTERS_Y, B_bitmap, CHAR_WIDTH, CHAR_HEIGHT, SOFT_GREEN_COLOR);
  displayArtillery_update_B_counter_display(INITIAL_COUNTER_VALUE, DRAW);

  // P and values
  display_drawBitmap(P_DIGIT_1_X, TOP_ROW_LETTERS_Y, P_bitmap, CHAR_WIDTH, CHAR_HEIGHT, SOFT_GREEN_COLOR);
  displayArtillery_update_P_counter_display(INITIAL_COUNTER_VALUE, DRAW);

  // W and values
  display_drawBitmap(W_DIGIT_1_X, TOP_ROW_LETTERS_Y, W_bitmap, CHAR_WIDTH, CHAR_HEIGHT, SOFT_GREEN_COLOR);
  displayArtillery_update_W_counter_display(INITIAL_WIND_SPEED, DRAW);
}

void displayArtillery_erase_top_segment() {
  display_drawBitmap(SCREEN_LEFT_EDGE, SCREEN_TOP_EDGE, line_bitmap, DISPLAY_WIDTH, LINE_HEIGHT, LIGHT_PURPLE_BG_COLOR);
  display_drawBitmap(SCREEN_LEFT_EDGE, TOP_SEGMENT_Y, top_segment_bitmap, DISPLAY_WIDTH, TOP_SEGMENT_HEIGHT,
                     LIGHT_PURPLE_BG_COLOR);
  display_drawBitmap(SCREEN_LEFT_EDGE, LINE_2_Y, line_bitmap, DISPLAY_WIDTH, LINE_HEIGHT, LIGHT_PURPLE_BG_COLOR);

  // B and values
  display_drawBitmap(B_DIGIT_1_X, TOP_ROW_LETTERS_Y, B_bitmap, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_PURPLE_BG_COLOR);
  displayArtillery_update_B_counter_display(INITIAL_COUNTER_VALUE, !DRAW);

  // P and values
  display_drawBitmap(P_DIGIT_1_X, TOP_ROW_LETTERS_Y, P_bitmap, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_PURPLE_BG_COLOR);
  displayArtillery_update_P_counter_display(INITIAL_COUNTER_VALUE, !DRAW);

  // W and values
  display_drawBitmap(W_DIGIT_1_X, TOP_ROW_LETTERS_Y, W_bitmap, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_PURPLE_BG_COLOR);
  displayArtillery_update_W_counter_display(INITIAL_WIND_SPEED, !DRAW);
}

void displayArtillery_draw_clouds(uint16_t cloud_1_x) {
  display_drawBitmap(CLOUD_X, CLOUD_Y, clouds_bitmap, CLOUD_WIDTH, CLOUD_HEIGHT, SOFT_YELLOW_COLOR);
  display_drawBitmap(CLOUD_X + CLOUD_X_SEPARATION, CLOUD_Y, clouds_bitmap, CLOUD_WIDTH, CLOUD_HEIGHT,
                     SOFT_YELLOW_COLOR);
}

void displayArtillery_timer_display(uint8_t count) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

  // Clear previous value
  display_drawBitmap(BOTTOM_DIGIT_1_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[prevTensPlace]), CHAR_WIDTH,
                     BOTTOM_DIGIT_HEIGHT, TERRAIN_GREEN_COLOR);
  display_drawBitmap(BOTTOM_DIGIT_2_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[prevOnesPlace]), CHAR_WIDTH,
                     BOTTOM_DIGIT_HEIGHT, TERRAIN_GREEN_COLOR);

  // Draw current value
  display_drawBitmap(BOTTOM_DIGIT_1_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[tensPlace]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     GREEN_NUMBERS_COLOR);
  display_drawBitmap(BOTTOM_DIGIT_2_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[onesPlace]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     GREEN_NUMBERS_COLOR);

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}
void displayArtillery_timer_erase(uint8_t count) {
  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

  // Clear previous value
  display_drawBitmap(BOTTOM_DIGIT_1_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[tensPlace]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     TERRAIN_GREEN_COLOR);
  display_drawBitmap(BOTTOM_DIGIT_2_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[onesPlace]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     TERRAIN_GREEN_COLOR);
}

void displayArtillery_update_B_counter_display(uint8_t count, bool draw) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

    // Determine what color to erase the previous number with
  uint16_t clearColor = (!draw) ? LIGHT_PURPLE_BG_COLOR : DISPLAY_BLACK;

  // Clear previous value
  display_drawBitmap(B_DIGIT_1_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevTensPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                     clearColor);
  display_drawBitmap(B_DIGIT_2_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevOnesPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                     clearColor);

  // Draw current value
  if (draw) {
    display_drawBitmap(B_DIGIT_1_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[tensPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                       SOFT_YELLOW_COLOR);
    display_drawBitmap(B_DIGIT_2_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[onesPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                       SOFT_YELLOW_COLOR);
  }

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}

void displayArtillery_update_P_counter_display(uint8_t count, bool draw) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

  // Determine what color to erase the previous number with
  uint16_t clearColor = (!draw) ? LIGHT_PURPLE_BG_COLOR : DISPLAY_BLACK;

  // Clear previous value
  display_drawBitmap(P_DIGIT_1_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevTensPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                     clearColor);
  display_drawBitmap(P_DIGIT_2_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevOnesPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                     clearColor);

  // Draw current value
  if (draw) {
    display_drawBitmap(P_DIGIT_1_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[tensPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                       SOFT_YELLOW_COLOR);
    display_drawBitmap(P_DIGIT_2_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[onesPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                       SOFT_YELLOW_COLOR);
  }

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}

void displayArtillery_update_W_counter_display(uint8_t count, bool draw) {
  static uint8_t prevTensPlace = 0;
  static uint8_t prevOnesPlace = 0;

  // Calculate the digits
  uint8_t onesPlace = count % 10;
  uint8_t tensPlace = (count - onesPlace) / 10;

    // Determine what color to erase the previous number with
  uint16_t clearColor = (!draw) ? LIGHT_PURPLE_BG_COLOR : DISPLAY_BLACK;

  // Clear previous value
  display_drawBitmap(W_DIGIT_1_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevTensPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                     clearColor);
  display_drawBitmap(W_DIGIT_2_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[prevOnesPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                     clearColor);

  // Draw current value
  if (draw) {
    display_drawBitmap(W_DIGIT_1_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[tensPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                       SOFT_YELLOW_COLOR);
    display_drawBitmap(W_DIGIT_2_X, TOP_ROW_DIGITS_Y, &(*counterBitmaps[onesPlace]), CHAR_WIDTH, CHAR_HEIGHT,
                       SOFT_YELLOW_COLOR);
  }

  // Update previous value
  prevTensPlace = tensPlace;
  prevOnesPlace = onesPlace;
}

void displayArtillery_flip_flag(bool direction) {
  const uint8_t *prev_flag_bitmap = (direction) ? (uint8_t *)&flag_L_bitmap : (uint8_t *)&flag_R_bitmap;
  const uint8_t *current_flag_bitmap = (direction) ? (uint8_t *)&flag_R_bitmap : (uint8_t *)&flag_L_bitmap;

  // Clear the previous flag and draw the new flag
  display_drawBitmap(FLAG_X, TOP_ROW_LETTERS_Y, prev_flag_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, DISPLAY_BLACK);
  display_drawBitmap(FLAG_X, TOP_ROW_LETTERS_Y, current_flag_bitmap, SIDE_IMG_WIDTH, FLAG_HEIGHT, SOFT_YELLOW_COLOR);
}

static void displayArtillery_player_1() {
  display_drawBitmap(PLAYER_1_X, player_one_ylocation, player_left_bitmap, PLAYER_WIDTH, PLAYER_HEIGHT,
                     SOFT_YELLOW_COLOR);
}

static void displayArtillery_player_2() {
  display_drawBitmap(PLAYER_2_X, player_two_ylocation, player_right_bitmap, PLAYER_WIDTH, PLAYER_HEIGHT,
                     SOFT_YELLOW_COLOR);
}

static void displayArtillery_player_1_turn() {
  display_drawBitmap(TURN_INDICATOR_1_X, player_one_ylocation + INDICATOR_OFFSET, turn_indicator_bitmap,
                     INDICATOR_WIDTH, INDICATOR_HEIGHT, PURPLE_MOUNTAIN_COLOR);
}

static void displayArtillery_player_2_turn() {
  display_drawBitmap(TURN_INDICATOR_2_X, player_two_ylocation + INDICATOR_OFFSET, turn_indicator_bitmap,
                     INDICATOR_WIDTH, INDICATOR_HEIGHT, PURPLE_MOUNTAIN_COLOR);
}

void displayArtillery_playerFlash(bool player1_turn, bool flash) {
  // Flash the specified player every 0.25 seconds
  if (player1_turn)
    (flash) ? displayArtillery_player_1_turn() : displayArtillery_player_1();
  else
    (flash) ? displayArtillery_player_2_turn() : displayArtillery_player_2();
}

void displayArtillery_drawPlayers() {
  display_drawBitmap(PLAYER_1_X, player_one_ylocation, player_left_bitmap, PLAYER_WIDTH, PLAYER_HEIGHT,
                     SOFT_YELLOW_COLOR);
  display_drawBitmap(PLAYER_2_X, player_two_ylocation, player_right_bitmap, PLAYER_WIDTH, PLAYER_HEIGHT,
                     SOFT_YELLOW_COLOR);
}

void displayArtillery_draw_score(uint8_t score1, uint8_t score2) {
  // Draw current value
  display_drawBitmap(SCORE_1_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[score1]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     GREEN_NUMBERS_COLOR);
  display_drawBitmap(SCORE_2_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[score2]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     GREEN_NUMBERS_COLOR);
}

void displayArtillery_erase_score(uint8_t score1, uint8_t score2) {
  // Draw current value
  display_drawBitmap(SCORE_1_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[score1]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     TERRAIN_GREEN_COLOR);
  display_drawBitmap(SCORE_2_X, BOTTOM_DIGITS_Y, &(*timerBitmaps[score2]), CHAR_WIDTH, BOTTOM_DIGIT_HEIGHT,
                     TERRAIN_GREEN_COLOR);
}
