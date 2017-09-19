#ifndef TRON_DEFINITIONS_H
#define TRON_DEFINITIONS_H

// Dependencies
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// Constant Values
#ifndef PI
#define PI 3.14159265358979323846
#endif

// Direction Enum
typedef enum dirAsNum {
  UP,
  DOWN,
  LEFT,
  RIGHT
} dirNum_t;

// Point Struct
typedef struct point {
  int x;
  int y;
  int occupant;
} point_t;

// Board Struct
// NOTE: a board is a *list* of *list*s where each element is a *pointer* to
//       a point struct
typedef point_t*** board_t;

// Direction enumeration
typedef struct direction {
  int8_t dX; // positive is right
  int8_t dY; // positive is down
} dir_t;

// Player Struct
// NOTE: aiSchema is 0 if the player is human controlled and the schema number
//       otherwise
typedef struct player {
  char *name;
  int playerNo;
  point_t *head;
  dir_t *dir;
  uint8_t directionChanged;
  uint8_t alive;
  uint8_t aiSchema;
} player_t;

// AllPlayers Struct
typedef struct allPlayers {
  player_t **all;
  int numHumans;
  int numRobots;
  int numPlayers;
  int randomness;
} allPlayers_t;

typedef struct graphics {
  SDL_Window *window;
  SDL_Renderer *renderer;
} graphics_t;

// GameStatus Struct
typedef struct gameStatus {
  board_t board;
  int boardDim;
  allPlayers_t *players;
  int playersAlive;
  graphics_t *graphics;
} gSt_t;

#endif
