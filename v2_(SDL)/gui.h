#ifndef GUI_H
#define GUI_H

// Dependencies
#include "definitions.h"
#include "util.h"
#include "player.h"

// Method Declarations
graphics_t *initGraphicsStruct();
uint8_t initGUI(graphics_t *graphics, int windowWidth, int windowHeight);
int handleEvent(gSt_t *gameStatus, SDL_Event *e);
void tryMoveHuman(gSt_t *gameStatus, int playerNo, dirNum_t newDirNum);
void dirFromDirection(dir_t *newDir, int direction);
void drawBoard(gSt_t *gameStatus);
void drawCell(SDL_Renderer *renderer, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b);
void freeGraphics(graphics_t *graphics);
#endif
