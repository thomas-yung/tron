#ifndef UTIL_H
#define UTIL_H

// Dependencies
#include "definitions.h"

// Method Declarations
void checkAllocFail(void *ptr, char *fromMethod);
void checkOtherPlayersAlive(gameStatus_t *game, point_t *head, int deadPlayer);
void decrementPlayersAlive(gameStatus_t *game);

#endif
