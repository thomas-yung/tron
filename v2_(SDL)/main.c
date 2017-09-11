// Dependencies
#include "definitions.h"
#include "util.h"

// Method Declarations
int main(int argc, char **argv);
void pollUserForParameters();
void initialise();
void gameLoop();
human_t **initHumans();
robot_t **initRobots();
board_t initBoard();
void freeMemory();

// Instance Variables
int numHumans;
int numRobots;
int aiSchema;
int FPS;
int holeFreq;
int boardDim;
int randomness;

gameStatus_t *gameStatus;


// Method Descriptions
// main executable
int main(int argc, char **argv) {
  if (argc == 8) {
    // Correct number of command-line arguments given
    numHumans = atoi(argv[1]);
    assert(numHumans >= 0 && numHumans <= 2);
    numRobots = atoi(argv[2]);
    aiSchema = atoi(argv[3]);
    FPS = atoi(argv[4]);
    holeFreq = atoi(argv[5]);
    boardDim = atoi(argv[6]);
    randomness = atoi(argv[7]);
  } else {
    // pollUserForParameters();
    printf("Let's pretend the parameters are in...\n");
    numHumans = 2;
    numRobots = 8;
    aiSchema = 1;
    FPS = 16;
    holeFreq = 10;
    boardDim = 100;
    randomness = 90;
  }

  initialise();

  gameLoop();

  freeMemory();
  return EXIT_SUCCESS;
}

void pollUserForParameters() {
  printf("Enter number of human players: ");
  scanf("%d", &numHumans);
  assert(numHumans >= 0 && numHumans <= 2);
  printf("Enter number of bots: ");
  scanf("%d", &numRobots);
  printf("Enter AI schema (0 to 1 inclusive)");
  scanf("%d", &aiSchema);
  printf("Enter refresh rate (Reccomended 16): ");
  scanf("%d", &FPS);
  printf("Enter frequency of holes (Reccomended 5): ");
  scanf("%d", &holeFreq);
  printf("Enter board height: ");
  scanf("%d", &boardDim);
  printf("Enter randomness / 100 (Higher means more AI mistakes): ");
  scanf("%d", &randomness);
  return;
}

void initialise() {
  // Initialise gameStatus memory
  gameStatus = calloc(1, sizeof(gameStatus_t));
  checkAllocFail(gameStatus, "main.initialise, gameStatus");

  human_t **allHuman = initHumans();

  robot_t **allBots = initRobots();

  allPlayers_t *allPlayers = calloc(1, sizeof(allPlayers_t));
  checkAllocFail(allPlayers, "main.initialise, allPlayers");
  allPlayers->humans = allHuman;
  allPlayers->robots = allBots;
  gameStatus->players = allPlayers;

  board_t gameBoard = initBoard();
  gameStatus->board = gameBoard;
  return;
}

human_t **initHumans() {
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

robot_t **initRobots() {
  robot_t **all = calloc(numRobots, sizeof(robot_t*));
  checkAllocFail(all, "main.initRobots, all");
  for (int i = 0; i < numRobots; i++) {
    robot_t *robot = calloc(1, sizeof(robot_t));
    checkAllocFail(robot, "main.initRobots, robot");
    robot->playerNo = numHumans + i + 1;
    robot->alive = 1;
    robot->randomness = randomness;
    all[i] = robot;
  }
  return all;
}

// NOTE: board_t == point_t*** (see definitions.h for explanation)
board_t initBoard() {
  board_t board = calloc(boardDim, sizeof(point_t**));
  checkAllocFail(board, "main.initBoard, board");
  for (int colNum = 0; colNum < boardDim; colNum++) {
    point_t **column = calloc(boardDim, sizeof(point_t*));
    checkAllocFail(column, "main.initBoard, column");
    for (int rowNum = 0; rowNum < boardDim; rowNum++) {
      point_t *point = calloc(1, sizeof(point_t));
      checkAllocFail(point, "main.initBoard, point");
      point->x = colNum;
      point->y = rowNum;
      point->occupant = 0;
      column[rowNum] = point;
    }
    board[colNum] = column;
  }
  return board;
}

void gameLoop() {
  printf("\nIn gameLoop\n\n");

  // while (1) {
  //   printf("Give x then y (boardDim=%d)\n", boardDim);
  //   int x;
  //   int y;
  //   scanf("%d", &x);
  //   scanf("%d", &y);
  //   printf("x: %d, y: %d, occupant: %d\n", ((gameStatus->board)[x][y])->x,((gameStatus->board)[x][y])->y,((gameStatus->board)[x][y])->occupant);
  // }

}

void freeMemory() {
  // Free allPlayers
  // NOTE: point_t* head is removed at the same time as board is removed
  allPlayers_t *allPlayers = gameStatus->players;

  human_t **allHumans = allPlayers->humans;

  for (int i = 0; i < numHumans; i++) {
    human_t *oneHuman = allHumans[i];
    free(oneHuman->name);
    free(oneHuman->dir);
    free(oneHuman);

  }
  free(allHumans);

  robot_t **allRobots = allPlayers->robots;
  for (int i = 0; i < numRobots; i++) {
    robot_t *oneRobot = allRobots[i];
    free(oneRobot->dir);
    free(oneRobot);
  }
  free(allRobots);

  free(allPlayers);

  // Free board (NOTE: WHY DOES THIS ONLY WORK IN REVERSE)
  board_t board = gameStatus->board;
  for (int col = 0; col < boardDim; col++) {
    for (int row = 0; row < boardDim; row++) {
      point_t *cell = board[col][row];
      // printf("\tHere 1     col=%d, row=%d\n", col, row);
      free(cell);
      // printf("\t  Here 2   col=%d, row=%d\n\n", col, row);
    }
    free(board[col]);
  }

  free(board);

  // Free gameStatus
  free(gameStatus);

  return;
}
