#ifndef PLAYER_H
#define PLAYER_H

// Dependencies
#include "aiSchemas.h"
#include "board.h"
#include "definitions.h"
#include "util.h"

// Method Declarations
player_t **initPlayers(int numHumans, int numRobots, int aiSchema);
void chooseDirection(player_t *player, gSt_t *game, int aiSchema, int randomness);
void setHumanDirection(player_t *player, dir_t *newDir);
void movePlayer(gSt_t *game, player_t *player);
void freePlayers(allPlayers_t *allPlayers);

#endif
