#ifndef TRON_PLAYER_H
#define TRON_PLAYER_H
#ifndef M_PI
#define M_PI           3.14159265358979323846


// Imported Files

#include "board.h"
#include "definitions.h"

// Methods
player_t* initialisePlayer(char* name, int number, int numPlayers);
void setPlayerDirection(player_t* player, int dx, int dy);
uint8_t movePlayer(board_t board, player_t* player, int holes);
void freePlayer(player_t* player);
void setRobotDirection(board_t board, robot_t* robot, int randomness);
void setRobotDirection4(board_t board, robot_t* robot, int randomness);

#endif
#endif
