#ifndef AISCHEMA_H
#define AISCHEMA_H

// Dependencies
#include "definitions.h"
#include "util.h"

// Method Declarations
void pickRandomValidDir(player_t *player, int randomness);
void maximiseSpaceAhead(player_t *player, gSt_t *gameStatus, int randomness);

#endif
