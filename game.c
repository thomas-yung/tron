// Included Files
#include <tgmath.h>
#include "game.h"

//Methods
// Declaration for method to draw players
void drawPlayers(board_t board, int numPlayers);

// Ouput the graphical representation of the current game state
void drawBoard(board_t board, int numPlayers) {
  drawPlayers(board, numPlayers);
}

// Returns x 'mod' 2.0 for floating point numbers
float mod2F(float x){
  return (x < 2) ? x : mod2F (x - 2.0);
}

// Returns absolute value for a number
float absF(float x){
  return x > 0 ? x : -1.0 * x;
}

void assignColour(float player, float numPlayers){
    float hue = 6.0 * player / numPlayers;
    float intermediate = 1.0 - (absF(mod2F (hue - 1.0)) );
    float r, g, b;
    if (hue <= 1) {
        r = 1;
        g = intermediate;
        b = 0;
    } else if (hue <= 2){
        r = intermediate;
        g = 1;
        b = 0;
    } else if (hue <= 3){
        r = 0;
        g = 1;
        b = intermediate;
    } else if (hue <= 4){
        r = 0;
        g = intermediate;
        b = 1;
    } else if (hue <= 5){
        r = intermediate;
        g = 0;
        b = 1;
    } else if (hue <= 6){
        r = 1;
        g = 0;
        b = intermediate;
    } else {
        r = 0;
        g = 0;
        b = 0;
    }
    glColor3f(r, g, b);
}

void drawPlayers(board_t board, int numPlayers) {
    for (int i = 0; i < BOARD_DIMENSION; i++) {
        for (int j = 0; j < BOARD_DIMENSION; j++) {
            if (board[i][j] == 0) {
                continue;
            } else {
                assignColour((float) board[i][j], (float) numPlayers);
                glBegin(GL_QUADS); // 2x2 pixels
                glVertex2f(i, j);
                glVertex2f(i + 1, j);
                glVertex2f(i + 1, j + 1);
                glVertex2f(i, j + 1);
                glEnd();
            }
        }
    }
}




void renderBitmapString(float x, float y, void *font, const char *string, float player, float numPlayers) {
    const char *c;
    assignColour(player, numPlayers);
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
