// Dependencies
#include "gui.h"

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
int handleEvent(gSt_t *gameStatus, SDL_Event *e) {
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
void tryMoveHuman(gSt_t *gameStatus, int playerNo, dirNum_t newDirNum) {
  int numHumans = gameStatus->players->numHumans;
  if (numHumans <= 0) {
    return;
  } else {
    dir_t *currentDir = ((gameStatus->players->all)[playerNo - 1])->dir;
    if (isValidDirNum(dirNumFromDir(currentDir), newDirNum)) {
      dir_t *newDir = dirFromDirNum(newDirNum);
      setHumanDirection((gameStatus->players->all)[playerNo - 1], newDir);
    }
  }
  return;
}

// Display the current board status
void drawBoard(gSt_t *gameStatus) {
  SDL_Renderer *renderer = gameStatus->graphics->renderer;
  int boardDim = gameStatus->boardDim;

  int windowWidth;
  int windowHeight;
  SDL_GetWindowSize(gameStatus->graphics->window, &windowWidth, &windowHeight);

  int numPlayers = gameStatus->players->numPlayers;

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
  for (int i = 0; i < numPlayers; i++) {
    player_t *onePlayer = (gameStatus->players->all)[i];
    if (onePlayer->alive) {
      int x = onePlayer->head->x * cellWidth;
      int y = onePlayer->head->y * cellHeight;
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
