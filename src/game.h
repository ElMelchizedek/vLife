#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "graphic.h"

// *Custom structs with defined types*
// Basic Entity: Use is required for any graphical or graphical-interacting variable (like the camera) to be manipulated in the game world
typedef struct entity
{
    void *thing;
    int form;
    int x;
    int y;
    int w;
    int h;
    int initX;
    int initY;
} entity;

// Key States: passed every cycle to updateGame, used for moving the camera.
typedef struct kStates 
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool start;
} kStates;

// Mouse States: passed every cycle to updateGame, used for flipping cell states before simulation start.
typedef struct mStates
{
    bool leftButton;
    bool rightButton;
    bool buttonDown;
    int x;
    int y;
} mStates;

// Cell Data: The data for the cell, which is paired with cellGraphics to form cellThing
typedef struct cellData
{
    int column;
    int row;
    bool lifeState;
    int aliveNeighbours;
    bool alreadyChanged;
} cellData;

// Combines together the data and the SDL_Rect to be put into an entity for a cell
typedef struct cellThing
{
    cellData* data;
    SDL_Rect* graphics;
} cellThing;

// Form macros
#define T_CAMERA    0
#define T_SDL_RECT  1
#define T_MTEXTURE  2
#define T_CELL 3

// Zoom macros
#define ZOOM_IN true
#define ZOOM_OUT false

// Cell macros
#define CELL_WIDTH 20
#define CELL_HEIGHT 20
#define CELL_X_PADDING 20
#define CELL_Y_PADDING 20

// Variables for other functions
extern struct kStates keyStates;
extern bool quit;

// Functions
entity* createEntity(void *selectedThing, int selectedType, int posX, int posY, int initW, int initH, entity** list, int* counter);
void entityUpdate(entity *selectedEntity, entity *camera);
void updateGame(entity *camera, entity** list);
void updateKeyStates(bool keyState, int keyChoice, entity** grid);
entity** initialiseCellGrid(entity** selectCellGrid, int selectLevelWidth, int selectLevelHeight, entity** list, int* counter, void*** addressList, int* addressCount);
void updateCellStates(bool state, int posX, int posY, entity** cellGrid);
void simulate(entity** grid);
void calculate(entity** grid);

#endif // GAME_H