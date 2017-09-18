// Dependencies
#include "human.h"

// Method Descriptions
// Initialise up to two human players of the game and combine them into a list
// which is returned to the caller.
human_t **initHumans(int numHumans, int numPlayers) {
  human_t **all = calloc(numHumans, sizeof(human_t*));
  checkPtrNull(all, "main.initHumans, all");
  for (int i = 0; i < numHumans; i++) {
    human_t *player = calloc(1, sizeof(human_t));
    checkPtrNull(player, "main.initHumans, player");
    printf("Player %d: enter your name...\n", (i + 1));
    char *name = calloc(20, sizeof(char));
    scanf("%s", name);
    player->name = name;
    player->playerNo = i + 1;
    dir_t *stDir = getStartDir(i + 1,  numPlayers);
    player->dir = stDir;
    player->alive = 1;
    all[i] = player;
  }
  return all;
}

// Move the human by one space in the direction it is facing
// The value returned indicates if the player survives this move
// 0 for death, non-zero for survival
int moveHuman(gameStatus_t *game, human_t *player, int boardDim) {
  point_t *head = player->head;
  dir_t *dir = player->dir;
  board_t board = game->board;
  int playerNo = player->playerNo;

  int newCol = head->x + dir->dX;
  int newRow = head->y + dir->dY;

  // printf("newRow = %d, newCol = %d\n", newRow, newCol);
  if (outOfBounds(newCol, newRow, boardDim)) {
    player->alive = 0;
    decrementPlayersAlive(game);
    return 0;
  } else {
    // printf("In bounds, Occ = %d\n", getOccupant(board, newCol, newRow));
    player->head = getPoint(board, newCol, newRow);
    if (!getOccupant(board, newCol, newRow)) {
      setOccupant(board, newCol, newRow, playerNo);
      player->directionChanged = 0;
      return 1;
    } else {
      player->alive = 0;
      decrementPlayersAlive(game);
      checkOtherPlayersAlive(game, player->head, playerNo);
      return 0;
    }
  }
}

// Sets human direction iff it is valid (not going back on itself)
void setHumanDirection(human_t *player, dir_t *newDir) {

  if (!(player->directionChanged)) {
    dir_t *current = player->dir;

    if (isValidDir(current, newDir)) {
      player->dir = newDir;
      player->directionChanged = 1;
      free(current);
    }
  }
  return;
}

// Free the memory used by the humans
void freeHumans(human_t **allHumans, uint8_t numHumans) {
  for (int i = 0; i < numHumans; i++) {
    human_t *oneHuman = allHumans[i];
    free(oneHuman->name);
    free(oneHuman->dir);
    free(oneHuman);
  }
  return;
}
