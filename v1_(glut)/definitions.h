#ifndef TRON_DEFINITIONS_H
#define TRON_DEFINITIONS_H

// Imported Files
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Constant Values


typedef uint8_t** board_t;

// Point Struct
typedef struct point {
  int x;
  int y;
} point_t;

// Direction enumeration
typedef struct direction {
  int dX; // 1 is right
  int dY; // 1 is down
} dir_t;

// Player Struct
typedef struct player {
  char *name;
  int playerNo;
  point_t *head;
  dir_t *dir;
  uint8_t directionChanged;
  uint8_t alive;
} player_t;

typedef player_t robot_t;
extern int BOARD_DIMENSION;
#endif
