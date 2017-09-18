// Dependencies
#include "aiSchemas.h"

// Method Descriptions
// A simple AI that picks a random move out of the valid ones available to it
// If there are no valid moves then the direction is not changed
// This AI is barely intelligent
void pickRandomValidDir(player_t *player, int randomness) {
  uint8_t numPossibleTurns = 0;
  dir_t **possibleTurns = calloc(3, sizeof(dir_t*));
  checkPtrNull(possibleTurns, "aiSchemas.c.pickRandomValidDir, possibleTurns");

  for (int8_t dX = -1; dX <= 1; dX++) {
    for (int8_t dY = -1; dY <= 1; dY++) {
      dir_t *tryDir = calloc(1, sizeof(dir_t));
      checkPtrNull(possibleTurns, "aiSchemas.c.pickRandomValidDir, tryDir");

      // Fill out possible directions
      if (!dX && dY) {
        // dX = 0 and dY != 0
        tryDir->dX = dX;
        tryDir->dY = dY;
        if (isValidDir(player->dir, tryDir)) {
          possibleTurns[numPossibleTurns] = tryDir;
          numPossibleTurns++;
        } else {
          free(tryDir);
        }
      } else if (dX && !dY) {
        // dX != 0 and dY == 0
        tryDir->dX = dX;
        tryDir->dY = dY;
        if (isValidDir(player->dir, tryDir)) {
          possibleTurns[numPossibleTurns] = tryDir;
          numPossibleTurns++;
        } else {
          free(tryDir);
        }
      }
    }
  }

  if (numPossibleTurns) {
    // There is at least one so randomly pick and set
    int chance = rand() % 101;
    if (chance < randomness) {
      int option = rand() % numPossibleTurns;
      player->dir = possibleTurns[option];

      // Free other options
      for (int i = 0; i < numPossibleTurns; i++) {
        if (i != option) {
          free(possibleTurns[i]);
        }
      }
    }
  }

  free(possibleTurns);
  return;
}

// This AI aims to find the maximum number of unoccupied squares ahead of (in
// a straight line from) itself.
// It considers different combinations of moves 10 iterations ahead to see
// what direction is best
// Parameters for this *may* include the number of moves ahead looked and the
// number of turns it executes in checking combinations
void maximiseSpaceAhead(player_t *player, gSt_t *gameStatus, int randomness) {

}
