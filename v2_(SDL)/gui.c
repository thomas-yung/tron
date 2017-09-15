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
          tryMoveHuman(gameStatus, 1, UP);
          return 0;
        case SDLK_DOWN:
          tryMoveHuman(gameStatus, 1, DOWN);
          return 0;
        case SDLK_LEFT:
          tryMoveHuman(gameStatus, 1, LEFT);
          return 0;
        case SDLK_RIGHT:
          tryMoveHuman(gameStatus, 1, RIGHT);
          return 0;
      }
    }
    return 0;
  }
}

// Attempt to move a specified human, doing nothing if the specified one does
// not exist
void tryMoveHuman(gameStatus_t *gameStatus, int playerNo, int direction) {
  printf("%d\n", direction);
  int numHumans = gameStatus->players->numHumans;
  if (numHumans <= 0) {
    return;
  } else if (numHumans == 1) {
    if (playerNo == 1) {
      dir_t *newDir = calloc(1, sizeof(dir_t));
      checkPtrNull(newDir, "gui.c.tryMoveHuman, newDir");
      dirFromDirection(newDir, direction);
      setHumanDirection((gameStatus->players->humans)[0], newDir);
    }
  } else if (numHumans == 2) {
    dir_t *newDir = calloc(1, sizeof(dir_t));
    checkPtrNull(newDir, "gui.c.tryMoveHuman, newDir");
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


// Free memory used by graphics
void freeGraphics(graphics_t *graphics) {
  SDL_DestroyRenderer(graphics->renderer);
	SDL_DestroyWindow(graphics->window);
	graphics->window = NULL;
	graphics->renderer = NULL;

	SDL_Quit();
}
