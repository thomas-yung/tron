#ifndef UTIL_H
#define UTIL_H

// Dependencies
#include "definitions.h"

// Method Declarations
void checkPtrNull(void *ptr, char *fromMethod);
void checkOtherPlayersAlive(gSt_t *game, point_t *head, int deadPlayer);
void decrementPlayersAlive(gSt_t *game);
uint8_t *playerNumToRGB(int numPlayers, int playerNo);
dir_t *getStartDir(int playerNo, int numPlayers);
int *getStartPos(int playerNo, int numPlayers, int boardDim);
int outOfBounds(int col, int row, int boardDim);
player_t *findWinner(gSt_t *gameStatus);
// int isValidDir(dir_t *current, dir_t *newDir);
int isValidDirNum(dirNum_t current, dirNum_t newDirNum);
dir_t *dirFromDirNum(dirNum_t dirNum);
dirNum_t dirNumFromDir(dir_t *dir);

#endif
