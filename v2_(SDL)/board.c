// Dependencies
#include "board.h"

// Method Descriptions
// Initialise the board, setting every cell to having an occupant of 0 (empty)
board_t initBoard(int boardDim) {
  board_t board = calloc(boardDim, sizeof(point_t**));
  checkPtrNull(board, "board.c.initBoard, board");

  for (int colNum = 0; colNum < boardDim; colNum++) {
    point_t **column = calloc(boardDim, sizeof(point_t*));
    checkPtrNull(column, "board,c.initBoard, column");

    for (int rowNum = 0; rowNum < boardDim; rowNum++) {
      point_t *point = calloc(1, sizeof(point_t));
      checkPtrNull(point, "board.c.initBoard, point");
      point->x = colNum;
      point->y = rowNum;
      point->occupant = 0;
      column[rowNum] = point;
    }

    board[colNum] = column;
  }
  
  return board;
}

void putPlayersOnBoard(gameStatus_t *gameStatus, int boardDim) {
  int numPlayers = gameStatus->playersAlive;
  board_t board = gameStatus->board;
  allPlayers_t *all =  gameStatus->players;

  int numHumans = all->numHumans;

  // Add humans
  for (int i = 0; i < numHumans; i++) {
    int playerNo = i + 1;
    int *coords = getStartPos(playerNo, numPlayers, boardDim);
    (all->humans)[i]->head = getPoint(board, coords[0], coords[1]);
    // First player in array (index=0) has a player number of 1
    setOccupant(board, coords[0], coords[1], i + 1);
    free(coords);
  }

  // Add robots
  for (int i = numHumans; i < numPlayers; i++) {
    // NOTE: if last human is player number 2, numHumans = 2 so first robot is 3
    int playerNo = i + 1;
    int *coords = getStartPos(playerNo, numPlayers, boardDim);
    (all->robots)[i - numHumans]->head = getPoint(board, coords[0], coords[1]);
    setOccupant(board, coords[0], coords[1], playerNo);
    free(coords);
  }

  return;
}


// Return the occupant number of a cell
int getOccupant(board_t board, int col, int row) {
  return (board[col][row])->occupant;
}

void setOccupant(board_t board, int col, int row, int occupant) {
  (board[col][row])->occupant = occupant;
  return;
}


// Return a pointer to a point (cell) on the board
point_t *getPoint(board_t board, int x, int y) {
  return board[x][y];
}


// Free the memory used by the board
void freeBoard(board_t board, int boardDim) {
  for (int col = 0; col < boardDim; col++) {
    for (int row = 0; row < boardDim; row++) {
      point_t *cell = board[col][row];
      free(cell);
    }
    free(board[col]);
  }
  return;
}
