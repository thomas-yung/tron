#ifndef GUI_H
#define GUI_H

// Dependencies
#include "definitions.h"
#include "util.h"
#include "human.h"

// Method Declarations
graphics_t *initGraphicsStruct();
uint8_t initGUI(graphics_t *graphics, int windowWidth, int windowHeight);
int handleEvent(gameStatus_t *gameStatus, SDL_Event *e);
void tryMoveHuman(gameStatus_t *gameStatus, int playerNo, int direction);
void dirFromDirection(dir_t *newDir, int direction);
void drawBoard(gameStatus_t *gameStatus, int boardDim);
void freeGraphics(graphics_t *graphics);
#endif
