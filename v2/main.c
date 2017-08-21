// Dependencies
#include "definitions.h"

// Method Declarations
int main(int argc, char **argv);
void initialise();

// Instance Variables
int numPeople;
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
    numPeople = atoi(argv[1]);
    assert(numPeople == 2);
    numRobots = atoi(argv[2]);
    aiSchema = atoi(argv[3]);
    FPS = atoi(argv[4]);
    holeFreq = atoi(argv[5]);
    boardDim = atoi(argv[6]);
    randomness = atoi(argv[7]);
  } else {
    pollUserForParameters();
  }

  initialise();

  gameLoop();

  freeMemory();
  return EXIT_SUCCESS;
}

pollUserForParameters() {
  printf("Enter number of players: ");
  scanf("%d", &numPeople);
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
}

void initialise() {
  // Initialise gameStatus memory
  gameStatus = calloc(1, sizeof(gameStatus_t));
  checkAllocFail(gameStatus, "main.initialise, gameStatus");

  human_t *allHuman = initHumans();
  robot_t *allBots = initRobots();

  allPlayers_t *allPlayers = calloc(1, sizeof(allPlayers_t));
  checkAllocFail(allPlayers, "main.initialise, allPlayers");
  allPlayers->players = allHuman;
  allPlayers->robots = allBots;
  gameStatus->players = allPlayers;

  board_t gameBoard = initBoard();
  gameStatus->board = gameBoard;
}

human_t *initHumans() {
  human_t *all = calloc(numPeople, sizeof(human_t*));
  checkAllocFail(all, "main.initHumans, all");
  for (int i = 0; i < numPeople; i++) {
    human_t *player = calloc(1, sizeof(human_t));
    checkAllocFail(player, "main.initHumans, player");
    printf("Player %d: enter your name...\n", (i + 1));
    char *name;
    scanf("%s\n", name);
    player->name = name;
    player->playerNo = i + 1;
    player->alive = 1;
    all[i] = player;
  }
  return all;
}

robot_t *initRobots() {

}

board_t initBoard() {
  // NOTE: board_t == point_t**
}
