// Dependencies
#include "robot.h"

// Method Descriptions
// Initialise the robot players and return a pointer to a list containing them
robot_t **initRobots(int numRobots, int numHumans, int randomness) {
  int numPlayers = numRobots + numHumans;
  robot_t **all = calloc(numRobots, sizeof(robot_t*));
  checkPtrNull(all, "robot.c.initRobots, all");
  for (int i = 0; i < numRobots; i++) {
    robot_t *robot = calloc(1, sizeof(robot_t));
    checkPtrNull(robot, "robot.c.initRobots, robot");
    robot->playerNo = numHumans + i + 1;
    robot->dir = getStartDir(numHumans + i + 1,  numPlayers);
    robot->alive = 1;
    robot->randomness = randomness;
    all[i] = robot;
  }
  return all;
}

// Autonomously decide and set the new direction
void chooseDirection(robot_t *player, gSt_t *gameStatus, int schema) {
  switch (schema) {
    case 0:
      pickRandomValidDir(player);
      return;
    case 1:
      maximiseSpaceAhead(player, gameStatus);
      return;
    default:
      printf("AI schema not applicable, choosing pickRandomValidDir\n");
      chooseDirection(player, gameStatus, 0);
      return;
  }
}

// Move the robot by one space in the direction it is facing
// The value returned indicates if the player survives this move
// 0 for death, non-zero for survival
int moveRobot(gSt_t *game, robot_t *player, int aiSchema) {
  chooseDirection(player, game, aiSchema);
  point_t *head = player->head;
  dir_t *dir = player->dir;
  board_t board = game->board;
  int playerNo = player->playerNo;

  int newCol = head->x + dir->dX;
  int newRow = head->y + dir->dY;

  if (outOfBounds(newCol, newRow, game->boardDim)) {
    player->alive = 0;
    decrementPlayersAlive(game);
    return 0;
  } else {
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

// Free the memory used by the robots
void freeRobots(robot_t **allRobots, int numRobots) {
  for (int i = 0; i < numRobots; i++) {
    robot_t *oneRobot = allRobots[i];
    free(oneRobot->dir);
    free(oneRobot);
  }
  return;
}
