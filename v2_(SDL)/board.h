#ifndef BOARD_H
#define BOARD_H

// Dependencies
#include "definitions.h"
#include "util.h"

// Method Declarations
board_t initBoard(int boardDim);
void putPlayersOnBoard(gameStatus_t *gameStatus, int boardDim);
int getOccupant(board_t board, int col, int row);
void setOccupant(board_t board, int col, int row, int occupant);
point_t *getPoint(board_t board, int x, int y);
void freeBoard(board_t board, int boardDim);

#endif
