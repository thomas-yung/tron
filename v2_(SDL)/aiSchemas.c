// Dependencies
#include "aiSchemas.h"

// Method Descriptions
// A helper method which finds all the valid dirNums and puts the total count in
// memory at the given pointer
dirNum_t *allValidTurns(player_t *player, int *numResults) {
  int numPossibleTurns = 0;
  dirNum_t *possibleTurns = calloc(3, sizeof(dir_t*));
  checkPtrNull(possibleTurns, "aiSchemas.c.allValidTurns, possibleTurns");

  dirNum_t current = dirNumFromDir(player->dir);

  for (dirNum_t i = UP; i <= RIGHT; i++) {
    if (isValidDirNum(current, i)) {
      possibleTurns[numPossibleTurns] = i;
      numPossibleTurns++;
    }
  }
  *numResults = numPossibleTurns;
  return possibleTurns;
}

// A helper method which finds all the valid dirNums that result in survival
// and puts the total count in memory at the given pointer
dir_t **allSurvivalTurns(player_t *player, int *numResults, gSt_t *g) {
  int numValid = 0;
  dirNum_t *allValid = allValidTurns(player, &numValid);
  int numSurv = 0;
  dir_t **allSurv = calloc(numValid, sizeof(dir_t*));
  checkPtrNull(allSurv, "aiSchemas.c.allSurvivalTurns, allSurv");

  for (int i = 0; i < numValid; i++) {
    dir_t *dir = dirFromDirNum(allValid[i]);
    int x = player->head->x + dir->dX;
    int y = player->head->y + dir->dY;

    if (!outOfBounds(x, y, g->boardDim)) {

      if (!getOccupant(g->board, x, y)) {
        // Destination is unoccupied
        allSurv[numSurv] = dir;
        numSurv++;
      }

    } else {
      free(dir);
    }
  }

  free(allValid);
  *numResults = numSurv;

  return allSurv;
}

// A simple AI that picks a random move out of the valid ones available to it
// If there are no valid moves then the direction is not changed
// This AI is barely intelligent
void pickRandomValidDir(player_t *player, int randomness) {
  int numCandidates;
  dirNum_t *candidateTurns = allValidTurns(player, &numCandidates);

  dir_t *current = player->dir;

  if (numCandidates) {
    // There is at least one so randomly pick and set
    int chance = rand() % 101;
    if (chance < randomness) {
      dirNum_t option = rand() % numCandidates;
      player->dir = dirFromDirNum(option);
      free(current);
    }
  }

  free(candidateTurns);
  return;
}

// A simple AI that picks a random move out of the valid ones available to it
// that result is the next move being survived
// If there are no valid moves then the direction is not changed
// This AI is barely intelligent
void pickRandomSurvDir(player_t *player, int randomness, gSt_t *g) {
  int numCandidates = 0;
  dir_t **candidateTurns = allSurvivalTurns(player, &numCandidates, g);

  dir_t *current = player->dir;

  if (numCandidates) {
    // There is at least one so randomly pick and set
    int chance = rand() % 101;
    int destX = player->head->x + player->dir->dX;
    int destY = player->head->y + player->dir->dY;

    int forceTurn = 0;

    // Staggered this way to avoid seg fault with getOccupant
    if (outOfBounds(destX, destY, g->boardDim)) {
      forceTurn = 1;
    } else {
      if (chance < randomness || getOccupant(g->board, destX, destY)) {
        forceTurn = 1;
      }
    }

    int option = -1;

    if (forceTurn) {
      option = rand() % numCandidates;
      player->dir = candidateTurns[option];
      free(current);
    }

    // Free unused options
    for (int i = 0; i < numCandidates; i++) {
      if (i != option) {
        free(candidateTurns[i]);
      }
    }

  }

  free(candidateTurns);
  return;
}


// A helper method for 'maximiseSpaceAhead' which returns a score representing
// the number of unoccupied squares in a straight line from the player after
// turning to face the direction given by dX and dY
int spacesAheadAfter(player_t *player, gSt_t *gameStatus, int dX, int dY) {
  int spaceAhead = 0;
  int destX = player->head->x + dX;
  int destY = player->head->y + dY;

  board_t board = gameStatus->board;
  int boardDim = gameStatus->boardDim;

  int keepChecking = 1;

  while (!outOfBounds(destX, destY, boardDim) && keepChecking) {
    if (!getOccupant(board, destX, destY)) {
      spaceAhead++;
      destX += dX;
      destY += dY;
    } else {
      keepChecking = 0;
    }
  }

  return spaceAhead;
}

// This AI aims to find the maximum number of unoccupied squares ahead of (in
// a straight line from) itself after turning different directions
// NOTE: In this case, direction is changed by altering the existing direction
//       values
void maximiseSpaceAhead(player_t *player, gSt_t *gameStatus) {
  dir_t *current = player->dir;
  int currentDX = current->dX;
  int currentDY = current->dY;

  // Score for continuing straight (followed by a random sequence of turns)
  int straightScore = spacesAheadAfter(player, gameStatus, currentDX, currentDY);

  // Score for turning relative left (followed by a random sequence of turns)
  int8_t tmpDX = currentDY;
  int8_t tmpDY = currentDX;
  int leftScore = spacesAheadAfter(player, gameStatus, tmpDX, tmpDY);

  // Score for turning relative right (followed by a random sequence of turns)
  tmpDX *= -1;
  tmpDY *= -1;
  int rightScore = spacesAheadAfter(player, gameStatus, tmpDX, tmpDY);

  // Random weighting (up to 50% increase)
  straightScore *= 1.0f + (rand() % 50) / 100.0f;
  leftScore *= 1.0f + (rand() % 50) / 100.0f;
  rightScore *= 1.0f + (rand() % 50) / 100.0f;

  // Pick best based off score
  if (straightScore >= leftScore) {
    if (straightScore >= rightScore) {
      // Straight is best
      return;
    } else {
      // Relative right is best
      current->dX = tmpDX;
      current->dY = tmpDY;
    }
  } else if (leftScore >= rightScore) {
    // Relative left is best
    current->dX = currentDY;
    current->dY = currentDX;
  } else {
    // Relative right is best
    current->dX = tmpDX;
    current->dY = tmpDY;
  }
  return;
}
