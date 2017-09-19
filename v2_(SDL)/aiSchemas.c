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

  *numResults = numSurv;

  return allSurv;
}

// A simple AI that picks a random move out of the valid ones available to it
// If there are no valid moves then the direction is not changed
// This AI is barely intelligent
void pickRandomValidDir(player_t *player, int randomness) {
  int numCandidates;
  dirNum_t *candidateTurns = allValidTurns(player, &numCandidates);

  if (numCandidates) {
    // There is at least one so randomly pick and set
    int chance = rand() % 101;
    if (chance < randomness) {
      dirNum_t option = rand() % numCandidates;
      player->dir = dirFromDirNum(option);
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

  if (numCandidates) {
    // There is at least one so randomly pick and set
    int chance = rand() % 101;
    if (chance < randomness) {
      int option = rand() % numCandidates;
      player->dir = candidateTurns[option];

      // Free other option
      for (int i = 0; i < numCandidates; i++) {
        if (i != option) {
          free(candidateTurns[i]);
        }
      }
    }
  }

  free(candidateTurns);
  return;
}


// A helper method for 'maximiseSpaceAhead' which recursively simulates 'm'
// moves in a random combination of turns and returns a score representing
// the number of unoccupied squares in a straight line from the player when it
// is at the end of 'm' moves
int spacesAheadAfter(player_t *player, board_t board, int m) {
  // if (!m) {`
  //   // m == 0 so count spaces in straight line
  //   int spaceAhead = 0;
  //
  //   while (getOccupant())
  // } else {
  //   // Recursively use function for each turn and pick max
  //
  // }`
  return 0;
}

// This AI aims to find the maximum number of unoccupied squares ahead of (in
// a straight line from) itself.
// It considers different combinations of moves 10 iterations ahead to see
// what direction is best
void maximiseSpaceAhead(player_t *player, gSt_t *gameStatus, int movesAhead) {
  // board_t board = gameStatus->board;

  //


}
