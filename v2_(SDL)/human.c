// Dependencies
#include "human.h"

// Method Descriptions
// Initialise up to two human players of the game and combine them into a list
// which is returned to the caller.
human_t **initHumans(int numHumans) {
  human_t **all = calloc(numHumans, sizeof(human_t*));
  checkAllocFail(all, "main.initHumans, all");
  for (int i = 0; i < numHumans; i++) {
    human_t *player = calloc(1, sizeof(human_t));
    checkAllocFail(player, "main.initHumans, player");
    printf("Player %d: enter your name...\n", (i + 1));
    char *name = calloc(20, sizeof(char));
    scanf("%s", name);
    player->name = name;
    player->playerNo = i + 1;
    player->alive = 1;
    all[i] = player;
  }
  return all;
}

// Move the human by one space in the direction it is facing
// The value returned indicates if the player survives this move
// 0 for death, non-zero for survival
int moveHuman(gameStatus_t *game, human_t *player) {
  point_t *head = player->head;
  dir_t *dir = player->dir;
  board_t board = game->board;
  int playerNo = player->playerNo;
  int newCol = head->x + dir->dX;
  int newRow = head->y + dir->dY;
  head = getPoint(board, newCol, newRow);
  if (getOccupant(board, newCol, newRow)) {
    setOccupant(board, newCol, newRow, playerNo);
    return 1;
  } else {
    player->alive = 0;
    decrementPlayersAlive(game);
    checkOtherPlayersAlive(game, head, playerNo);
    return 0;
  }
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
