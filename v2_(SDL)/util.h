#ifndef UTIL_H
#define UTIL_H

// Dependencies
#include "definitions.h"

// Method Declarations
void checkPtrNull(void *ptr, char *fromMethod);
void checkOtherPlayersAlive(gameStatus_t *game, point_t *head, int deadPlayer);
void decrementPlayersAlive(gameStatus_t *game);
uint8_t *playerNumToRGB(int numPlayers, int playerNo);
dir_t *getStartDir(int playerNo, int numPlayers);

#endif
