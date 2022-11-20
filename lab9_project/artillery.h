/* The same player structure will be used for all players in the game,
so this enum is used to identify the type of player */
typedef enum { PLAYER_TYPE_PLAYER, PLAYER_TYPE_ENEMY } player_type_t;

/* This struct contains all information about the bullet */
typedef struct {
  // Current state (the 'enum' will be defined in your missile.c file, so it's
  // just declared as an integer type here)
  int32_t currentState;

  // Starting x,y of missile
  uint16_t x_origin;
  uint16_t y_origin;

  // Ending x,y of missile, and the total length from origin to destination.
  uint16_t x_dest;
  uint16_t y_dest;
  uint16_t total_length;

  // While flying, this tracks the current length of the flight path
  double length;

  // Used to track the current x,y of missile
  int16_t x_current;
  int16_t y_current;

} bullet_t;

/* This struct contains all information about a player */
typedef struct {
  // player type (player, enemy)
  player_type_t type;

  bullet_t bullet;

  // Current state (the 'enum' will be defined in your missile.c file, so it's
  // just declared as an integer type here)
  int32_t currentState;

  // Used to track the current x,y of missile
  int16_t x_current;
  int16_t y_current;

  bool is_turn;

} player_t;
