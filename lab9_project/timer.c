#include "timer.h"
#include "displayArtillery.h"

static uint8_t score_timer;
static uint8_t timer;
static uint8_t timer_tick_period;
static uint8_t flash_tick_period;
static bool timer_expired;
static bool button_hide;
static bool button_isDisplayed;
static bool pt_isDisplayed;
static bool power_isActive;
static bool angle_isActive;
static bool score_timer_done;

void timer_init(float period_seconds) {
  timer = 30;
  timer_tick_period = (uint8_t)1 / period_seconds;
  flash_tick_period = timer_tick_period / 4;
  timer_angle_setActive();
  button_hide = false;
}

void timer_tick(bool player1_turn) {
  static uint16_t secCounter = 0;
  static uint16_t scoreSecCounter = 0;
  static uint16_t buttonFlashSecCounter = 0;
  static uint16_t playerFlashSecCounter = 0;

  // Wait 5 seconds when displaying the score
  if (!score_timer_done) {
    if (scoreSecCounter == timer_tick_period) {
      score_timer--;
      scoreSecCounter = 0;
    }
    if (!score_timer)
      score_timer_done = true;

    scoreSecCounter++;
  }

  // Countdown the turn timer
  if (secCounter == timer_tick_period || timer_expired) {
    if (!timer) {
      timer_expired = true;
      displayArtillery_timer_display(timer);
    } else if (!timer_expired) {
      timer--;
      displayArtillery_timer_display(timer);
    }

    secCounter = 0;
  }

  // Flash the buttons every fourth of a second
  if (buttonFlashSecCounter == flash_tick_period) {
    if (!button_hide)
      if (!button_isDisplayed)
        (angle_isActive) ? displayArtillery_angle() : displayArtillery_power();
      else
        displayArtillery_eraseButtons();
    buttonFlashSecCounter = 0;
    button_isDisplayed = !button_isDisplayed;
  }

  // Flash the turn indicator every fourth of a second
  if (playerFlashSecCounter == flash_tick_period) {
    displayArtillery_playerFlash(player1_turn, pt_isDisplayed);
    playerFlashSecCounter = 0;
    pt_isDisplayed = !pt_isDisplayed;
  }

  secCounter++;
  buttonFlashSecCounter++;
  playerFlashSecCounter++;
}

void timer_angle_setActive() {
  angle_isActive = true;
  power_isActive = false;
}
void timer_power_setActive() {
  angle_isActive = false;
  power_isActive = true;
}

void timer_hide_button(bool hide) { button_hide = hide; }

void timer_start_score_timer() {
  score_timer = 5;
  score_timer_done = false;
}
bool timer_score_timer_done() { return score_timer_done; }

void timer_start_turn_timer() {
  timer = 30;
  timer_expired = false;
  displayArtillery_timer_display(timer); // Necessary only for explicitly displaying the 30 mark
}

void timer_stop_turn_timer() {
  timer_expired = true;
  displayArtillery_timer_erase(timer);
}

bool timer_isExpired() { return timer_expired; }