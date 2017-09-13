// Dependencies
#include "definitions.h"
#include "util.h"
#include "human.h"
#include "robot.h"
#include "board.h"
#include <SDL2/SDL.h>

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

  human_t **allHuman = initHumans(numHumans);

  robot_t **allBots = initRobots(numRobots, numHumans, randomness);

  allPlayers_t *allPlayers = calloc(1, sizeof(allPlayers_t));
  checkAllocFail(allPlayers, "main.initialise, allPlayers");
  allPlayers->numHumans = numHumans;
  allPlayers->humans = allHuman;
  allPlayers->numRobots = numRobots;
  allPlayers->robots = allBots;
  gameStatus->players = allPlayers;

  board_t gameBoard = initBoard(boardDim);
  gameStatus->board = gameBoard;

  gameStatus->playersAlive = numHumans + numRobots;
  return;
}

void gameLoop() {
  printf("\nIn gameLoop\n\n");

  SDL_Window *window = NULL;
  SDL_Surface *screenSurface = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL_Init error: %s\n", SDL_GetError());
  } else {
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_SHOWN);
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    } else {
      //Get window surface
      screenSurface = SDL_GetWindowSurface( window );

      //Fill the surface white
      SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

      //Update the surface
      SDL_UpdateWindowSurface( window );

      //Wait two seconds
      SDL_Delay( 2000 );

      //Destroy window
      SDL_DestroyWindow( window );

      //Quit SDL subsystems
      SDL_Quit();
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

}

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

  // Free gameStatus
  free(gameStatus);

  return;
}
