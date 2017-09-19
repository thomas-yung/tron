// Dependencies
#include "player.h"

// Constants
#define movesAhead 10

// Initialise players and return a list with them in
player_t **initPlayers(int numHumans, int numRobots, int aiSchema) {
  int numPlayers = numHumans + numRobots;

  player_t **all = calloc(numPlayers, sizeof(player_t*));
  checkPtrNull(all, "player.c.initPlayers, all");

  for (int i = 0; i < numPlayers; i++) {
    player_t *player = calloc(1, sizeof(player_t));
    checkPtrNull(player, "player.c.initPlayers, player");
    player->playerNo = i + 1;
    player->dir = getStartDir(i + 1, numPlayers);
    player->directionChanged = 0;
    player->alive = 1;
    char *name = calloc(20, sizeof(char));

    if (i < numHumans) {
      printf("Player %d: enter your name...\n", (i + 1));
      checkPtrNull(name, "player.c.initPlayers, name");
      scanf("%s", name);
      player->name = name;
      player->aiSchema = 0;
    } else {
      strcpy(name, "Robot #");
      char num[3];
      sprintf(num, "%d", i + 1);
      strcat(name, num);
      player->name = name;
      player->aiSchema = aiSchema;
    }

    all[i] = player;
  }
  return all;
}

// Direct the direction-choosing to the correct schema
void chooseDirection(player_t *player, gSt_t *game, int aiSchema, int randomness) {
  switch (aiSchema) {
    case 0:
      printf("Choose direction should not be used for human\n");
      return;
    case 1:
      pickRandomValidDir(player, randomness);
      return;
    case 2:
      pickRandomSurvDir(player, randomness, game);
      return;
    case 3:
      maximiseSpaceAhead(player, game);
      return;
    default:
      printf("AI schema not applicable, choosing pickRandomValidDir\n");
      chooseDirection(player, game, 0, randomness);
      return;
  }
}

// Set the direction of the player (used for human players)
// Sets direction assuming it is valid (not going back on itself)
void setHumanDirection(player_t *player, dir_t *newDir) {
  dir_t *current = player->dir;
  if (!(player->directionChanged) && !(player->aiSchema)) {
    player->dir = newDir;
    player->directionChanged = 1;
  } else {

  }
  free(current);
  return;
}


// Move the player once
void movePlayer(gSt_t *game, player_t *player) {
  int aiSchema = player->aiSchema;
  if (aiSchema) {
    chooseDirection(player, game, aiSchema, game->players->randomness);
  }

  point_t *head = player->head;
  dir_t *dir = player->dir;
  board_t board = game->board;
  int playerNo = player->playerNo;

  int newCol = head->x + dir->dX;
  int newRow = head->y + dir->dY;

  if (outOfBounds(newCol, newRow, game->boardDim)) {
    player->alive = 0;
    decrementPlayersAlive(game);
    return;
  } else {
    player->head = getPoint(board, newCol, newRow);
    if (!getOccupant(board, newCol, newRow)) {
      setOccupant(board, newCol, newRow, playerNo);
      player->directionChanged = 0;
      return;
    } else {
      player->alive = 0;
      decrementPlayersAlive(game);
      checkOtherPlayersAlive(game, player->head, playerNo);
      return;
    }
  }
}

// Free the memory used by the players
void freePlayers(allPlayers_t *allPlayers) {
  int numPlayers = allPlayers->numPlayers;
  for (int i = 0; i < numPlayers; i++) {
    player_t *onePlayer = (allPlayers->all)[i];
    free(onePlayer->name);
    free(onePlayer->dir);
    free(onePlayer);
  }
  free(allPlayers->all);
  return;
}
