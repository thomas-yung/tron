#ifndef ROBOT_H
#define ROBOT_H

// Dependencies
#include "aiSchemas.h"
#include "board.h"
#include "definitions.h"
#include "util.h"

// Method Declarations
robot_t **initRobots(int numRobots, int numHumans, int randomness);
void chooseDirection(robot_t *player, gameStatus_t *gameStatus, int schema);
int moveRobot(gameStatus_t *game, robot_t *player, int aiSchema, int boardDim);
void freeRobots(robot_t **allRobots, int numRobots);

#endif
