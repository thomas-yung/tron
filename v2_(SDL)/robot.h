#ifndef ROBOT_H
#define ROBOT_H

// Dependencies
#include "aiSchemas.h"
#include "board.h"
#include "definitions.h"
#include "util.h"

// Method Declarations
robot_t **initRobots(int numRobots, int numHumans, int randomness);
void chooseDirection(robot_t *player, gSt_t *gameStatus, int schema);
int moveRobot(gSt_t *game, robot_t *player, int aiSchema);
void freeRobots(robot_t **allRobots, int numRobots);

#endif
