#ifndef TRON_DEFINITIONS_H
#define TRON_DEFINITIONS_H

// Dependencies
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Constant Values


// Point Struct
typedef struct point {
  int x;
  int y;
  int occupant;
} point_t;

// Board Struct
// NOTE: a board is a *list* of *list*s where each element is a *pointer* to
//       a point struct
typedef (point_t***) board_t;

// Direction enumeration
typedef struct direction {
  int dX; // positive is right
  int dY; // positive is down
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
  human_t **humans;
  robot_t **robots;
} allPlayers_t;

// GameStatus Struct
typedef struct gameStatus {
  board_t *board;
  allPlayers_t *players;
} gameStatus_t;

#endif
