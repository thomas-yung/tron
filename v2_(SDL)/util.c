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
void checkOtherPlayersAlive(gameStatus_t *game, point_t *head, int deadPlayer) {
  allPlayers_t *all = game->players;

  // Check human players
  human_t **allHumans = all->humans;
  int numHumans = all->numHumans;
  for (int i = 0; i < numHumans; i++) {
    human_t *oneHuman = allHumans[i];
    if (oneHuman->playerNo >= deadPlayer) {
      return;
    } else {
      if (head == oneHuman->head) {
        oneHuman->alive = 0;
        decrementPlayersAlive(game);
      }
    }
  }

  // Check robot players
  robot_t **allRobots = all->robots;
  int numRobots = all->numRobots;
  for (int i = 0; i < numRobots; i++) {
    robot_t *oneRobot = allRobots[i];
    if (oneRobot->playerNo >= deadPlayer) {
      return;
    } else {
      if (head == oneRobot->head) {
        oneRobot->alive = 0;
        decrementPlayersAlive(game);
      }
    }
  }

  return;
}

// Decrement the number of players alive
void decrementPlayersAlive(gameStatus_t *game) {
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
int findWinner(gameStatus_t *gameStatus) {
  int numHumans = gameStatus->players->numHumans;
  int numRobots = gameStatus->players->numRobots;

  for (int i = 0; i < numHumans; i++) {
    human_t *oneHuman = (gameStatus->players->humans)[i];
    if (oneHuman->alive) {
      return i;
    }
  }

  for (int i = 0; i < numRobots; i++) {
    robot_t *oneRobot = (gameStatus->players->robots)[i];
    if (oneRobot->alive) {
      return numHumans + i + 1;
    }
  }
  printf("Couldn't find winner\n");
  return -1;
}
