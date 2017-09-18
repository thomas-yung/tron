#ifndef HUMAN_H
#define HUMAN_H

// Dependencies
#include "board.h"
#include "definitions.h"
#include "util.h"

// Method Declarations
human_t **initHumans(int numHumans, int numPlayers);
int moveHuman(gSt_t *game, human_t *player);
void freeHumans(human_t **allHumans, uint8_t numHumans);
void setHumanDirection(human_t *player, dir_t *newDir);

#endif
