#ifndef CONFIG_H
#define CONFIG_H


#define CONFIG_GAME_TIMER_PERIOD 50.0E-3

#define CONFIG_MAX_PLAYERS 2

// Speed of bullet
#define CONFIG_GRAVITY_ACCELERATION -.1

// How fast explosion radius increases/decreases per second
#define CONFIG_EXPLOSION_RADIUS_CHANGE_PER_SECOND 30
#define CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK                                \
  (CONFIG_EXPLOSION_RADIUS_CHANGE_PER_SECOND * CONFIG_GAME_TIMER_PERIOD)

#define CONFIG_COLOR_BULLET DISPLAY_BLACK

#define CONFIG_EXPLOSION_MAX_RADIUS 25

#endif /* CONFIG_H */