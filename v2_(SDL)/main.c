// Dependencies
#include "definitions.h"
#include "util.h"
#include "player.h"
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
int numPlayers;
int aiSchema;
int FPS;
int holeFreq;
int boardDim;
int randomness;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

gSt_t *gameStatus;


// Method Descriptions
// main executable
int main(int argc, char **argv) {
  if (argc == 8) {
    // Correct number of command-line arguments given
    numHumans = atoi(argv[1]);
    assert(numHumans >= 0 && numHumans <= 2);
    numRobots = atoi(argv[2]);
    numPlayers = numHumans + numRobots;
    aiSchema = atoi(argv[3]);
    FPS = atoi(argv[4]);
    holeFreq = atoi(argv[5]);
    boardDim = atoi(argv[6]);
    randomness = atoi(argv[7]);
  } else {
    // pollUserForParameters();
    printf("Let's pretend the parameters are in...\n");
    numHumans = 0;
    numRobots = 100;
    numPlayers = numHumans + numRobots;
    aiSchema = 1;
    FPS = 16;
    holeFreq = 10;
    boardDim = 100;
    randomness = 20;
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
  printf("Enter randomness up to 100pc (what pc of the time does the AI turn): ");
  scanf("%d", &randomness);
  return;
}

void initialise() {
  // Initialise gameStatus memory
  gameStatus = calloc(1, sizeof(gSt_t));
  checkPtrNull(gameStatus, "main.initialise, gameStatus");

  // Initialise all the players
  allPlayers_t *allPlayers = calloc(1, sizeof(allPlayers_t));
  checkPtrNull(allPlayers, "main.initialise, allPlayers");
  allPlayers->all = initPlayers(numHumans, numRobots, aiSchema);
  allPlayers->numHumans = numHumans;
  allPlayers->numRobots = numRobots;
  allPlayers->numPlayers = numPlayers;
  allPlayers->randomness = randomness;
  gameStatus->players = allPlayers;

  // Initialise the board
  board_t gameBoard = initBoard(boardDim);
  gameStatus->board = gameBoard;
  gameStatus->boardDim = boardDim;

  gameStatus->playersAlive = numPlayers;

  putPlayersOnBoard(gameStatus);

  // Initialise graphics
  gameStatus->graphics = initGraphicsStruct();
  if (!initGUI(gameStatus->graphics, SCREEN_WIDTH, SCREEN_HEIGHT)) {
    printf("Could not initialise GUI, quitting...\n");
    exit(EXIT_FAILURE);
  }

  // Initialise random number generator
  srand(time(NULL));

  return;
}

void gameLoop() {
  printf("\nStarting game, (in gameLoop)\n\n");

  // Initialise library, create window and renderer
  // and set texture filtering (to linear)

  // Draw initial board
  drawBoard(gameStatus);

  // Initialise event holder
  SDL_Event e;

  int quit = 0;
  // main loop
  while (gameStatus->playersAlive > 1 && !quit) {
    while (SDL_PollEvent(&e)) {
      quit = handleEvent(gameStatus, &e);
    }

    // Simulate one move
    for (int i = 0; i < numPlayers; i++) {
      player_t *onePlayer = (gameStatus->players->all)[i];
      if (onePlayer->alive) {
        movePlayer(gameStatus, onePlayer);
      }
    }

    // Display the current board
    drawBoard(gameStatus);
    SDL_Delay(50);
  }

  int alive = gameStatus->playersAlive;

  if (alive == 1) {
    player_t *winner = findWinner(gameStatus);
    printf("Player \"%s\" won\n", winner->name);
  } else if (alive == 0) {
    printf("Tie\n");
  }

}

void freeMemory() {
  // Free allPlayers
  // NOTE: point_t* head is removed at the same time as board is removed
  allPlayers_t *allPlayers = gameStatus->players;
  freePlayers(allPlayers);
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
