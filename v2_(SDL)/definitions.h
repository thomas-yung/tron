#ifndef TRON_DEFINITIONS_H
#define TRON_DEFINITIONS_H

// Dependencies
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


// Constant Values
#ifndef PI
#define PI 3.14159265358979323846
#endif

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
typedef struct human {
  char *name;
  int playerNo;
  point_t *head;
  dir_t *dir;
  uint8_t directionChanged;
  uint8_t alive;
  uint8_t schema;
} human_t;

// Robot Struct
typedef struct robot {
  int playerNo;
  point_t *head;
  dir_t *dir;
  uint8_t directionChanged;
  uint8_t alive;
  int randomness;
} robot_t;

// AllPlayers Struct
typedef struct allPlayers {
  int numHumans;
  human_t **humans;
  int numRobots;
  robot_t **robots;
} allPlayers_t;

typedef struct graphics {
  SDL_Window *window;
  SDL_Renderer *renderer;
} graphics_t;

// GameStatus Struct
typedef struct gameStatus {
  board_t board;
  allPlayers_t *players;
  int playersAlive;
  graphics_t *graphics;
} gameStatus_t;

#endif
