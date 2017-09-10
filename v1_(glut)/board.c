// Included Files
#include "board.h"

// Methods
// Initialise a new game board in memory
board_t initialiseBoard(void){
  board_t board = (board_t) malloc(BOARD_DIMENSION * sizeof(uint8_t*));
  for (int i = 0; i < BOARD_DIMENSION; ++i) {
    board[i] = calloc(BOARD_DIMENSION, sizeof(uint8_t));
  }
  return board;
}

// Returns 0 if the board cell is unoccupied or a value greater than 0 if it is
uint8_t isOccupied(board_t board, int x, int y){
  return board[x][y];
}

// Set the occupant of a cell
void setOccupied(board_t board, int x, int y, player_t* player){
  board[x][y] = player->playerNo;
}

// Free the memory used by the board in memory
void freeBoard(board_t board){
  for (int i = 0; i < BOARD_DIMENSION; ++i) {
    free(board[i]);
  }
  free(board);
}
