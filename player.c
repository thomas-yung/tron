// Imported Files
#include "player.h"
#include <stdlib.h>
#include  <math.h>


// Methods
// Set the head position of the player
void setPlayerHead(player_t *player, int x, int y) {
  player->head->x = x;
  player->head->y = y;
}

// Set the direction of the player
void setPlayerDirection(player_t *player, int dx, int dy) {
  if (dx != -player->dir->dX && dy != -player->dir->dY && !player->directionChanged) {
    player->dir->dX = dx;
    player->dir->dY = dy;
    player->directionChanged = 1;
  }
}

// Get the direction struct from coordinates
dir_t* getDirection(int x, int y){
  int centre = BOARD_DIMENSION / 2;
  dir_t* direction = malloc(sizeof(direction));
  if (x < centre && y <= centre){
    direction->dX = 1;
    direction->dY = 0;
  } else if (x >= centre && y < centre){
    direction->dX = 0;
    direction->dY = 1;
  } else if (x > centre && y >= centre){
    direction->dX = -1;
    direction->dY = 0;
  } else {
    direction->dX = 0;
    direction->dY = -1;
  }
  return direction;
}

// Initialise a player in memory
player_t *initialisePlayer(char *name, int num, int numPlayers) {
  player_t *player = malloc(sizeof(player_t));

  player->name = name;
  player->playerNo = num;
  player->alive = 1;
  int posX, posY;

  posX = BOARD_DIMENSION * ( 0.5 - cos((num - 1) * 2 * M_PI / numPlayers) * 0.375 );
  posY = BOARD_DIMENSION * ( 0.5 - sin((num - 1) * 2 * M_PI / numPlayers) * 0.375 );

  player->head = malloc(sizeof(player_t));
  setPlayerHead(player, posX, posY);

  player->dir = getDirection(posX, posY);

  return player;
}

uint8_t isValidMove(board_t board, int newX, int newY){
  return !(newX < 0 || newX >= BOARD_DIMENSION
             || newY < 0 || newY >= BOARD_DIMENSION
             || isOccupied(board, newX, newY));
}

// Simple AI that randomly chooses a move
void randomValidMove(board_t board, robot_t* robot) {
    int dx = robot->dir->dX;
    int dy = robot->dir->dY;
    uint8_t frontEmpty = isValidMove(board, robot->head->x + dx,
                                     robot->head->y + dy);

    uint8_t leftEmpty = isValidMove(board, robot->head->x - dy,
                                    robot->head->y + dx);
    uint8_t rightEmpty = isValidMove(board, robot->head->x + dy,
                                     robot->head->y - dx);
    uint8_t numValid = frontEmpty + leftEmpty + rightEmpty;
    dir_t *validDir = malloc((numValid) * sizeof(dir_t));

    dir_t direction;
    direction.dX = dx;
    direction.dY = dy;
    validDir[0] = direction;
    if (leftEmpty) {
        dir_t direction;
        direction.dX = dx;
        direction.dY = dy;
        validDir[0] = direction;
        if (leftEmpty) {
            dir_t direction;
            direction.dX = -dy;
            direction.dY = dx;
            validDir[frontEmpty] = direction;
        }
        if (rightEmpty) {
            dir_t direction;
            direction.dX = dy;
            direction.dY = -dx;
            validDir[leftEmpty + frontEmpty] = direction;
        }
        if (numValid > 0) {
            direction = validDir[rand() % (numValid)];
            setPlayerDirection(robot, direction.dX, direction.dY);
        }
        free(validDir);
    }
}

void setRobotDirection(board_t board, robot_t* robot, int randomness){
    int dx = robot->dir->dX;
    int dy = robot->dir->dY;
    uint8_t frontEmpty = isValidMove(board, robot->head->x + dx, robot->head->y + dy);
    uint8_t leftEmpty = isValidMove(board, robot->head->x - dy, robot->head->y + dx);
    uint8_t rightEmpty = isValidMove(board, robot->head->x + dy, robot->head->y - dx);
    // uint8_t front2Empty = isValidMove(board, robot->head->x + dx + dx, robot->head->y + dy + dy);
    // uint8_t left2Empty = isValidMove(board, robot->head->x - dy - dy, robot->head->y + dx + dx);
    // uint8_t right2Empty = isValidMove(board, robot->head->x + dy + dy, robot->head->y - dx - dx);
    // check forward left and right 1 and 2 steps ; choose valid move if any occupied;
    if (!leftEmpty || !frontEmpty || !rightEmpty ){
        randomValidMove(board, robot);
    } else { // otherwise move straight and 1% change to randomly move in another direction
        int r = rand() % 100;
        if (r < randomness){
            randomValidMove(board, robot);
        }
    }
  }


float setRobotDirection4_(board_t board, robot_t* robot, int iteration){

  point_t* head = robot->head;
  dir_t* dir = robot->dir;
  if (iteration <= 0){
    float spaceAhead = 0;

    for (int i = 0; head->x + dir->dX * i < BOARD_DIMENSION
                  && head->y + dir->dY * i < BOARD_DIMENSION
                  ; i++ , spaceAhead += 0.5) {
      if (isOccupied(board, head->x + dir->dX * i,head->y + dir->dY * i)){
        return spaceAhead;
      }
    }
    return spaceAhead;
  } else if (head->x + dir->dX >= BOARD_DIMENSION
            || head->x + dir->dX < 0
            || head->y + dir->dY >= BOARD_DIMENSION
            || head->y + dir->dY < 0
            || isOccupied(board, head->x + dir->dX, head->y + dir->dY)) {
    return 0;
  } else {
    float value = 1.0;
    movePlayer(board, robot, 0);

    int dX = dir->dX;
    int dY = dir->dY;

    value += 0.75 * setRobotDirection4_(board, robot, iteration - 1);

    dir->dX = dY;
    dir->dY = dX;
    value += 0.75 * setRobotDirection4_(board, robot, iteration - 1);

    dir->dX = -dY;
    dir->dY = -dX;
    value += 0.75 * setRobotDirection4_(board, robot, iteration - 1);

    dir->dX = dX;
    dir->dY = dY;
    board[head->x][head->y] = 0;

    head->x -= dir->dX;
    head->y -= dir->dY;

    return value;
  }
}

float randomFloat(int randomness) {
  float value = 100.0 - (rand() % (randomness+1));
  return value / 100.0;
}

void setRobotDirection4(board_t board, robot_t* robot, int randomness){
    dir_t* dir = robot->dir;

    int numIterations = 10 - (randomness / 10);

    int dX = dir->dX;
    int dY = dir->dY;

    float result_forward =  setRobotDirection4_(board, robot, numIterations);

    dir->dX = dY;
    dir->dY = dX;
    float result_turn1 = randomFloat(randomness)
                            * setRobotDirection4_(board, robot, numIterations);

    dir->dX = -dY;
    dir->dY = -dX;
    float result_turn2 = randomFloat(randomness)
                            * setRobotDirection4_(board, robot, numIterations);

    if (result_forward >= result_turn1 && result_forward >= result_turn2){
      dir->dX = dX;
      dir->dY = dY;
    } else if (result_turn1 >= result_forward && result_turn1 >= result_turn2){
      dir->dX = dY;
      dir->dY = dX;
    } else {
    }
  }

uint8_t movePlayer(board_t board, player_t *player, int holes) {
    if (player->alive) {
        int newX = player->head->x + player->dir->dX;
        int newY = player->head->y + player->dir->dY;
        if (!isValidMove(board, newX, newY)) {
            return 0;
        }
        setPlayerHead(player, newX, newY);
        if (holes > 0) {
            int r = rand() % holes;
            if (r > 0) {
                setOccupied(board, newX, newY, player);
            }
        } else {
            setOccupied(board, newX, newY, player);
        }
        player->directionChanged = 0;
        return 1;
    }
    return 0;
}

// Free the memory used by a player
void freePlayer(player_t *player) {
  free(player->head);
  free(player->name);
  free(player->dir);
  free(player);
}
