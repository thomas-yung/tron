#ifndef TRON_GAME_H
#define TRON_GAME_H

// Imported Files
#include <GL/glut.h>
#include "definitions.h"

// Methods
void drawBoard(board_t board, int numPlayers);
void renderBitmapString(float x, float y,void *font, const char *string, float players, float numPlayers);

#endif
