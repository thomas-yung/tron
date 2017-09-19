#ifndef AISCHEMA_H
#define AISCHEMA_H

// Dependencies
#include "board.h"
#include "definitions.h"
#include "util.h"

// Method Declarations
dirNum_t *allValidTurns(player_t *player, int *numResults);
dir_t **allSurvivalTurns(player_t *player, int *numResults, gSt_t *g);

void pickRandomValidDir(player_t *player, int randomness);
void pickRandomSurvDir(player_t *player, int randomness, gSt_t *g);

int spacesAheadAfter(player_t *player, gSt_t *gameStatus, int dX, int dY);
void maximiseSpaceAhead(player_t *player, gSt_t *gameStatus);

#endif
