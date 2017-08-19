// Imported Files
#include <GL/glut.h>
#include <memory.h>
#include <time.h>
#include "game.h"
#include "player.h"
#include "definitions.h"

// Method Declarations
void display_callback(void);
void reshape_callback(int, int);
void timer_callback(int);
void specialKeyToDir(int key, int x, int y);
void keyToDir(unsigned char key, int x, int y);
void init(void);

// Instance Variables
static player_t** players;
static board_t board;
int numPlayers;
int numPeople;
int numRobots;
int FPS;
int holes;
int BOARD_DIMENSION;
int randomness;

// Methods
// Main executable
int main(int argc, char **argv) {
    // Random seed for random variables
    srand(time(NULL));
    // Assigning command line args to variables
    if (argc >= 7){
      numPeople = atoi(argv[1]);
      numRobots = atoi(argv[2]);
      FPS = atoi(argv[3]);
      holes = atoi(argv[4]);
      BOARD_DIMENSION = atoi(argv[5]);
      randomness = atoi(argv[6]);
    } else {
      printf("Enter number of players: ");
      scanf("%d", &numPeople);
      printf("Enter number of bots: ");
      scanf("%d", &numRobots);
      printf("Enter refresh rate (Reccomended 16): ");
      scanf("%d", &FPS);
      printf("Enter frequency of holes (Reccomended 5): ");
      scanf("%d", &holes);
      printf("Enter randomness / 100 (Higher means more AI mistakes): ");
      scanf("%d", &randomness);
      printf("Enter board height: ");
      scanf("%d", &BOARD_DIMENSION);
    }
    numPlayers = numPeople + numRobots;
    players = malloc((numPlayers)*sizeof(player_t));

    // Graphics (GLUT) initialisation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("TRONG");
    init();
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutTimerFunc(0, timer_callback, 0);
    glutKeyboardFunc(keyToDir);

    // Enable arrow keys if there are 2 human players
    if (numPeople == 2) {
        glutSpecialFunc(specialKeyToDir);
    }

    // Launch graphics
    glutMainLoop();
    return EXIT_SUCCESS;
}

// Initialise game
void init() {
  glClearColor(0, 0, 0, 1);
  for (int i = 1; i <= numPlayers; i++){
    char* playerName = calloc(10, sizeof(char));
    sprintf(playerName, "Player %d", i);
    players[i - 1] = initialisePlayer(playerName, i, numPlayers);
  }
  board = initialiseBoard();
}

// Pause the system
void waitFor (unsigned int secs) {
  unsigned int retTime = time(0) + secs;   // Get finishing time.
  while (time(0) < retTime);               // Loop until it arrives.
}

void drawRectangle(float x, float y, float l, float w, float r, float g, float b){
    glColor3f(r, g, b);
    glBegin(GL_QUADS); // 2x2 pixels
    glVertex2f(x - w / 2, y - l / 2);
    glVertex2f(x + w / 2, y - l / 2);
    glVertex2f(x + w / 2, y + l / 2);
    glVertex2f(x - w / 2, y + l / 2);
    glEnd();
}

void display_callback() {
    glClear(GL_COLOR_BUFFER_BIT);
    int alive = 0;
    for (int i = 0; i < numPlayers; i ++){
        if (players[i]->alive){
            alive++;
            // if (i > numPeople - 1){
            //     setRobotDirection(board, players[i], randomness);
            // }
            if (i > numPeople - 1){
              setRobotDirection4(board, players[i], randomness);
            }
        }
    }
    for (int i = 0;i < numPlayers; ++i) {
        players[i]->alive = movePlayer(board, players[i], holes);
        for (int j = 0; !players[i]->alive && j < i; ++j) {
             int x = players[i]->head->x + players[i]->dir->dX;
             int y = players[i]->head->y + players[i]->dir->dY;
             if (x == players[j]->head->x && y == players[j]->head->y){
                 players[j]->alive = 0;
             }
        }
    }
    if (alive > 1){
        drawBoard(board,numPlayers);
    } else if (alive == 0) {
        int r = rand()%numPlayers;
        renderBitmapString(BOARD_DIMENSION/2 + 2,BOARD_DIMENSION/2,GLUT_BITMAP_HELVETICA_18,"Tis a Tie",  (float)(r), (float)numPlayers);
    } else {
        int i = 0;
        for (; !players[i]->alive; ++i) {}
        char *string = malloc(15 * sizeof(char));
        strcpy(string, players[i]->name);
        strcat(string, " Won!");
        drawBoard(board, numPlayers);
        drawRectangle(BOARD_DIMENSION / 2 + BOARD_DIMENSION / 15, BOARD_DIMENSION / 2 - BOARD_DIMENSION / 105,
                      BOARD_DIMENSION / 25 + BOARD_DIMENSION / 200, BOARD_DIMENSION / 7 + BOARD_DIMENSION / 30, 0.4,
                      0.4, 0.4);
        drawRectangle(BOARD_DIMENSION / 2 + BOARD_DIMENSION / 15, BOARD_DIMENSION / 2 - BOARD_DIMENSION / 105,
                      BOARD_DIMENSION / 25 + BOARD_DIMENSION / 60, BOARD_DIMENSION / 7 + BOARD_DIMENSION / 60, 0.7, 0.7,
                      0.7);
        drawRectangle(BOARD_DIMENSION / 2 + BOARD_DIMENSION / 15, BOARD_DIMENSION / 2 - BOARD_DIMENSION / 105,
                      BOARD_DIMENSION / 25, BOARD_DIMENSION / 7, 1, 1, 1);


        renderBitmapString(BOARD_DIMENSION / 2, BOARD_DIMENSION / 2, GLUT_BITMAP_HELVETICA_18, string, (float) (i + 1),
                           (float) numPlayers);
        free(string);
        glutSwapBuffers();
        waitFor(6);
        freeBoard(board);
        for (int i = 0; i < numPlayers; i++) {
            freePlayer(players[i]);
        }
        free(players);
        glutDestroyWindow(0);
        exit(EXIT_SUCCESS);
    }
  glutSwapBuffers();
}


void reshape_callback(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Top-left is origin
  glOrtho(0, BOARD_DIMENSION, BOARD_DIMENSION, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void timer_callback(int x) {
  glutPostRedisplay();
  glutTimerFunc(1000 / FPS, timer_callback, 0);
}

void specialKeyToDir(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      setPlayerDirection(players[1], 0, -1);
      break;
    case GLUT_KEY_DOWN:
      setPlayerDirection(players[1], 0, 1);
      break;
    case GLUT_KEY_LEFT:
      setPlayerDirection(players[1], -1, 0);
      break;
    case GLUT_KEY_RIGHT:
      setPlayerDirection(players[1], 1, 0);
      break;
    default:
    perror("specialKeyToDir invalid key");
  }
}

// Returns direction from key input
void keyToDir(unsigned char key, int x, int y) {
  switch (key) {
    case 'w':
    case 'W':
      setPlayerDirection(players[0], 0, -1);
      break;
    case 'd':
    case 'D':
      setPlayerDirection(players[0], 1, 0);
      break;
    case 's':
    case 'S':
      setPlayerDirection(players[0], 0, 1);
      break;
    case 'a':
    case 'A':
      setPlayerDirection(players[0], -1, 0);
      break;
      default:
          return;
    // Invalid key input, do nothing
  }
}
