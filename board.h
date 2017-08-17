#ifndef TRON_BOARD_H
#define TRON_BOARD_H

// Included Files
#include "definitions.h"

// Methods
board_t initialiseBoard(void);
uint8_t isOccupied(board_t board, int x, int y);
void setOccupied(board_t board, int x, int y, player_t* player);
void freeBoard(board_t board);
void printBoard(board_t board);

#endif
