// Dependencies
#include "definitions.h"
#include "util.h"
#include "human.h"
#include "robot.h"
#include "board.h"
#include "gui.h"

// Method Declarations
int main(int argc, char **argv);
void pollUserForParameters();
void initialise();
void gameLoop();
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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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
  checkPtrNull(gameStatus, "main.initialise, gameStatus");

  human_t **allHuman = initHumans(numHumans);

  robot_t **allBots = initRobots(numRobots, numHumans, randomness);

  allPlayers_t *allPlayers = calloc(1, sizeof(allPlayers_t));
  checkPtrNull(allPlayers, "main.initialise, allPlayers");
  allPlayers->numHumans = numHumans;
  allPlayers->humans = allHuman;
  allPlayers->numRobots = numRobots;
  allPlayers->robots = allBots;
  gameStatus->players = allPlayers;

  board_t gameBoard = initBoard(boardDim);
  gameStatus->board = gameBoard;

  gameStatus->playersAlive = numHumans + numRobots;

  gameStatus->graphics = initGraphicsStruct();

  // Place players on board and assign colours
  
  return;
}

void gameLoop() {
  printf("\nStarting game, (in gameLoop)\n\n");

  // Initialise library, create window and renderer
  // and set texture filtering (to linear)
  if (!initGUI(gameStatus->graphics, SCREEN_WIDTH, SCREEN_HEIGHT)) {
    printf("Could not initialise GUI, quitting...\n");
    exit(EXIT_FAILURE);
  }

  // Initialise event holder
  SDL_Event e;

  int quit = 0;
  // main loop
  while (gameStatus->playersAlive > 1 && !quit) {

    while (SDL_PollEvent(&e)) {
      quit = handleEvent(gameStatus, &e);
    }

    // Simulate one move
    for (int i = 0; i < numHumans; i++) {
      human_t *oneHuman = (gameStatus->players->humans)[i];
      if (oneHuman->alive) {
        moveHuman(gameStatus, oneHuman);
      }
    }
    for (int i = 0; i < numRobots; i++) {
      robot_t *oneRobot = (gameStatus->players->robots)[numHumans + i];
      if (oneRobot->alive) {
        moveRobot(gameStatus, oneRobot, aiSchema);
      }
    }

    // set render colour and clear window
    // draw rects
    // update

  }
}



  // while (1) {
  //   printf("Give x then y (boardDim=%d)\n", boardDim);
  //   int x;
  //   int y;
  //   scanf("%d", &x);
  //   scanf("%d", &y);
  //   printf("x: %d, y: %d, occupant: %d\n", ((gameStatus->board)[x][y])->x,((gameStatus->board)[x][y])->y,((gameStatus->board)[x][y])->occupant);
  // }



void freeMemory() {
  // Free allPlayers
  // NOTE: point_t* head is removed at the same time as board is removed
  allPlayers_t *allPlayers = gameStatus->players;

  human_t **allHumans = allPlayers->humans;
  freeHumans(allHumans, numHumans);
  free(allHumans);

  robot_t **allRobots = allPlayers->robots;
  freeRobots(allRobots, numRobots);
  free(allRobots);

  free(allPlayers);


  // Free board
  board_t board = gameStatus->board;
  freeBoard(board, boardDim);
  free(board);

  // Free graphics
  graphics_t *graphics = gameStatus->graphics;
  freeGraphics(graphics);
  free(graphics);

  // Free gameStatus
  free(gameStatus);

  return;
}
