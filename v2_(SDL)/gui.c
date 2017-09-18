// Dependencies
#include "gui.h"

// Direction Enum
enum directionEnum {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

// Method Descriptions
// Initialise the struct holding pointers for gui utilities
graphics_t *initGraphicsStruct() {
  graphics_t *graphics = calloc(1, sizeof(graphics_t));
  checkPtrNull(graphics, "gui.h.initGraphicsStruct, graphics");
  graphics->window = NULL;
  graphics->renderer = NULL;
  return graphics;
}

// Initialise the SDL library and window in which the game is shown
uint8_t initGUI(graphics_t *graphics, int windowWidth, int windowHeight) {
  uint8_t success = 1;

  // Initialise library
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = 0;
  } else {
    //Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

    // Create window
    graphics->window = SDL_CreateWindow("tron BY thomas-yung", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (graphics->window == NULL) {
      printf("Window could not be created, SDL Error: %s\n", SDL_GetError());
      success = 0;
    } else {
      // Create renderer for window
      graphics->renderer = SDL_CreateRenderer(graphics->window, -1, SDL_RENDERER_ACCELERATED);
      if (graphics->renderer == NULL) {
        printf("Renderer could not be created, SDL Error: %s\n", SDL_GetError());
        success = 0;
      }
    }
  }

  return success;
}

// Handle an event, updating the game if necessary
// Returns 1 if the event is a quit
int handleEvent(gameStatus_t *gameStatus, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return 1;
  } else {
    if (e->type == SDL_KEYDOWN) {
      switch(e->key.keysym.sym) {
        case SDLK_w:
          tryMoveHuman(gameStatus, 1, UP);
          return 0;
        case SDLK_a:
          tryMoveHuman(gameStatus, 1, LEFT);
          return 0;
        case SDLK_s:
          tryMoveHuman(gameStatus, 1, DOWN);
          return 0;
        case SDLK_d:
          tryMoveHuman(gameStatus, 1, RIGHT);
          return 0;
        case SDLK_UP:
          tryMoveHuman(gameStatus, 2, UP);
          return 0;
        case SDLK_DOWN:
          tryMoveHuman(gameStatus, 2, DOWN);
          return 0;
        case SDLK_LEFT:
          tryMoveHuman(gameStatus, 2, LEFT);
          return 0;
        case SDLK_RIGHT:
          tryMoveHuman(gameStatus, 2, RIGHT);
          return 0;
      }
    }
    return 0;
  }
}

// Attempt to move a specified human, doing nothing if the specified one does
// not exist
void tryMoveHuman(gameStatus_t *gameStatus, int playerNo, int direction) {
  int numHumans = gameStatus->players->numHumans;
  if (numHumans <= 0) {
    return;
  } else {
    dir_t *newDir = calloc(1, sizeof(dir_t));
    checkPtrNull(newDir, "gui.c.tryMoveHuman, newDir");
    dirFromDirection(newDir, direction);
    setHumanDirection((gameStatus->players->humans)[playerNo - 1], newDir);
  }
  return;
}

// Allocates the fields of the dir_t based on given direction
void dirFromDirection(dir_t *newDir, int direction) {
  switch (direction) {
    case UP:
      newDir->dX = 0;
      newDir->dY = -1;
      return;
    case DOWN:
      newDir->dX = 0;
      newDir->dY = 1;
      return;
    case LEFT:
      newDir->dX = -1;
      newDir->dY = 0;
      return;
    case RIGHT:
      newDir->dX = 1;
      newDir->dY = 0;
      return;
  }
}

// Display the current board status
void drawBoard(gameStatus_t *gameStatus, int boardDim) {
  SDL_Renderer *renderer = gameStatus->graphics->renderer;


  int windowWidth;
  int windowHeight;
  SDL_GetWindowSize(gameStatus->graphics->window, &windowWidth, &windowHeight);

  int numPlayers = gameStatus->players->numHumans + gameStatus->players->numRobots;

  int cellWidth = windowWidth / boardDim;
  int cellHeight = windowHeight / boardDim;

  //Clear screen to black
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(renderer);

  //Render cells of grid
  for (int col = 0; col < boardDim; col++) {
    for (int row = 0; row < boardDim; row++) {
      int cellOccupant = getOccupant(gameStatus->board, col, row);
      if (cellOccupant) {
        int x = col * cellWidth;
        int y = row * cellHeight;
        uint8_t *rgb = playerNumToRGB(numPlayers, cellOccupant);
        drawCell(renderer, x, y, cellWidth, cellHeight, rgb[0], rgb[1], rgb[2]);
        free(rgb);
      }
    }
  }

  // Paint heads white
  int numHumans = gameStatus->players->numHumans;
  int numRobots = gameStatus->players->numRobots;

  for (int i = 0; i < numHumans; i++) {
    human_t *oneHuman = (gameStatus->players->humans)[i];
    if (oneHuman->alive) {
      int x = oneHuman->head->x * cellWidth;
      int y = oneHuman->head->y * cellHeight;
      drawCell(renderer, x, y, cellWidth, cellHeight, 0xFF, 0xFF, 0xFF);
    }
  }

  for (int i = 0; i < numRobots; i++) {
    robot_t *oneRobot = (gameStatus->players->robots)[i];
    if (oneRobot->alive) {
      int x = oneRobot->head->x * cellWidth;
      int y = oneRobot->head->y * cellHeight;
      drawCell(renderer, x, y, cellWidth, cellHeight, 0xFF, 0xFF, 0xFF);
    }
  }

  // Update screen
  SDL_RenderPresent(renderer);

return;
}

// Draw one cell of the board
void drawCell(SDL_Renderer *renderer, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b) {
  SDL_Rect cell = {x, y, w, h};
  SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
  SDL_RenderFillRect(renderer, &cell);
  return;
}


// Free memory used by graphics
void freeGraphics(graphics_t *graphics) {
  SDL_DestroyRenderer(graphics->renderer);
	SDL_DestroyWindow(graphics->window);
	graphics->window = NULL;
	graphics->renderer = NULL;

	SDL_Quit();
}
