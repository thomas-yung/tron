// Dependencies
#include "util.h"

// Method Descriptions
// Check if a pointer is null and exit with failure if so
// Used after memory allocation
void checkPtrNull(void *ptr, char *fromMethod) {
  if (!ptr) {
    printf("\nEXIT_FAILURE @ %s\n", fromMethod);
    exit(EXIT_FAILURE);
  }
}

// Upon a death, check all players with smaller player numbers than the
// deceased to see if their head is in the same place as the deceased
// Eliminate other player if this is the case
// Players with a higher number have not moves yet thus do not need to be
// checked
void checkOtherPlayersAlive(gSt_t *game, point_t *head, int deadPlayer) {
  player_t **allPlayers = game->players->all;
  int numPlayers = game->players->numPlayers;

  for (int i = 0; i < numPlayers; i++) {
    player_t *onePlayer = allPlayers[i];
    if (onePlayer->playerNo >= deadPlayer) {
      return;
    } else {
      if (head == onePlayer->head) {
        onePlayer->alive = 0;
        decrementPlayersAlive(game);
      }
    }
  }

  return;
}

// Decrement the number of players alive
void decrementPlayersAlive(gSt_t *game) {
  game->playersAlive--;
  return;
}

// Give an array of the RGB values for a given player
uint8_t *playerNumToRGB(int numPlayers, int playerNo) {
  uint8_t *rgb = calloc(3, sizeof(uint8_t));
  checkPtrNull(rgb, "util.c.playerNumToRGB, rgb");

  int maxColor = 0xFF;
  float para = maxColor / (numPlayers / 4.0f);


  // Red value
  if (playerNo > numPlayers / 4.0f && playerNo < numPlayers / 2.0f) {
    rgb[0] = maxColor - (playerNo - numPlayers / 4.0f) * para;
  } else if (playerNo < numPlayers / 4.0f) {
    rgb[0] = maxColor;
  } else {
    rgb[0] = 0;
  }

  // Green value
  if (playerNo >= 0 && playerNo < numPlayers / 4.0f) {
    rgb[1] = playerNo * para;
  } else if (playerNo > 3 * numPlayers/4) {
    rgb[1] = maxColor - (playerNo -  (3.0f * numPlayers/4.0f)) * para;
  } else {
    rgb[1] = maxColor;
  }

  // Blue value
  if ((playerNo > numPlayers / 2.0f) && (playerNo < 3.0f * numPlayers / 4.0f)) {
    rgb[2] = (playerNo - numPlayers / 2.0f) * para;
  } else if (playerNo <= numPlayers / 2.0f) {
    rgb[2] = 0;
  } else {
    rgb[2] = maxColor;
  }

  return rgb;
}

// Returns a dir_t for the starting direction of a player
dir_t *getStartDir(int playerNo, int numPlayers) {
  float one8th = numPlayers / 8.0f;
  dir_t *dir = calloc(1, sizeof(dir_t));
  int adjPN = playerNo - 1;

  if (adjPN >= one8th * 7 || adjPN < one8th) {
    dir->dX = 0;
    dir->dY = 1;
  } else if (adjPN >= one8th && adjPN < one8th * 3) {
    dir->dX = -1;
    dir->dY = 0;
  } else if (adjPN >= one8th * 3 && adjPN < one8th * 5) {
    dir->dX = 0;
    dir->dY = -1;
  } else {
    dir->dX = 1;
    dir->dY = 0;
  }
  return dir;
}

// Returns the starting position for a player given their player number
int *getStartPos(int playerNo, int numPlayers, int boardDim) {
  float adjNo = playerNo - 1;
  float r = 0.375 * boardDim;

  int *pair = calloc(2, sizeof(int));
  pair[0] = boardDim/2.0f - r * cos(2.0f * PI * (adjNo / (float) numPlayers));
  pair[1] = boardDim/2.0f - r * sin(2.0f * PI * (adjNo / (float) numPlayers));

  return pair;
}

// Returns 0 iff the column and row are wihin the sides of the board and
// 1 if the point is put of bounds
int outOfBounds(int col, int row, int boardDim) {
  return (col < 0 || row < 0 || col >= boardDim || row >= boardDim);
}

// Finds the only alive player
player_t *findWinner(gSt_t *gameStatus) {
  int numPlayers = gameStatus->players->numPlayers;

  for (int i = 0; i < numPlayers; i++) {
    player_t *onePlayer = (gameStatus->players->all)[i];
    if (onePlayer->alive) {
      return onePlayer;
    }
  }

  printf("Couldn't find winner\n");
  return NULL;
}

// A method stating if a direction change is valid
// A move is valid if the player does not turn back on itself
// int isValidDir(dir_t *current, dir_t *newDir) {
//   int8_t currentDX = current->dX;
//   int8_t currentDY = current->dY;
//
//   if (!currentDX) {
//     // Player is facing vertically
//     if (currentDY != -1 * (newDir->dY)) {
//       return 1;
//     }
//   } else if (!currentDY) {
//     // Player is facing horizontally
//     if (currentDX != -1 * (newDir->dX)) {
//       return 1;
//     }
//   }
//   return 0;
// }

// Returns 0 if the dirNums are invalid as succcessive turns or there is
// no change
int isValidDirNum(dirNum_t current, dirNum_t newDirNum) {
  int8_t total = current + newDirNum;
  return (total != 1) && (total != 5) && (current != newDirNum);
}

// Allocates the fields of the dir_t based on given direction enum
dir_t *dirFromDirNum(dirNum_t dirNum) {
  dir_t *newDir = calloc(1, sizeof(dir_t));
  checkPtrNull(newDir, "util.c.dirFromDirNum, newDir");
  switch (dirNum) {
    case UP:
      newDir->dX = 0;
      newDir->dY = -1;
      break;
    case DOWN:
      newDir->dX = 0;
      newDir->dY = 1;
      break;
    case LEFT:
      newDir->dX = -1;
      newDir->dY = 0;
      break;
    case RIGHT:
      newDir->dX = 1;
      newDir->dY = 0;
      break;
  }
  return newDir;
}

dirNum_t dirNumFromDir(dir_t *dir) {
  int8_t x = dir->dX;
  int8_t y = dir->dY;
  if (!x) {
    // Player is facing vertically
    if (y < 0) {
      return UP;
    } else {
      return DOWN;
    }
  } else if (!y) {
    // Player is facing horizontally
    if (x < 0) {
      return LEFT;
    } else {
      return RIGHT;
    }
  }
  printf("dirNumFromDir failed, returning 0\n");
  return 0;
}
