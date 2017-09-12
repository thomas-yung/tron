// Dependencies
#include "util.h"

// Method Descriptions
// Check if a pointer is null and exit with failure if so
// Used after memory allocation
void checkAllocFail(void *ptr, char *fromMethod) {
  if (!ptr) {
    printf("\nEXIT_FAILURE @ %s\n", fromMethod);
    exit(EXIT_FAILURE);
  }
}

// Upon a death, check all players with smaller player numbers than the
// deceased to see if their head is in the same place as the deceased
// Eliminate other player if this is the case
// Players with a higher number have not moves yet thus do not need to be
// checked
void checkOtherPlayersAlive(gameStatus_t *game, point_t *head, int deadPlayer) {
  allPlayers_t *all = game->players;

  // Check human players
  human_t **allHumans = all->humans;
  int numHumans = all->numHumans;
  for (int i = 0; i < numHumans; i++) {
    human_t *oneHuman = allHumans[i];
    if (oneHuman->playerNo >= deadPlayer) {
      return;
    } else {
      if (head == oneHuman->head) {
        oneHuman->alive = 0;
        decrementPlayersAlive(game);
      }
    }
  }

  // Check robot players
  robot_t **allRobots = all->robots;
  int numRobots = all->numRobots;
  for (int i = 0; i < numRobots; i++) {
    robot_t *oneRobot = allRobots[i];
    if (oneRobot->playerNo >= deadPlayer) {
      return;
    } else {
      if (head == oneRobot->head) {
        oneRobot->alive = 0;
        decrementPlayersAlive(game);
      }
    }
  }

  return;
}

// Decrement the number of players alive
void decrementPlayersAlive(gameStatus_t *game) {
  game->playersAlive--;
  return;
}
