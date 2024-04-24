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
    float zoom;
} entity;

// Moveable Texture: Joins an SDL_Texture and the corresponding SDL_Rect it will be mapped onto, for ease of use when being altered
typedef struct mTexture 
{
    SDL_Texture *texture;
    SDL_Rect destinationRect;
} mTexture;

// Transform + Coordinates: Used by centreGraphic() to give the transform values and coordinates to centre an entity, in a simple manner
typedef struct transCoords
{
    int x;
    int y;
    int w;
    int h;
} transCoords;

// Camera States: Modified upon SDL_KEYDOWN event whenever that occurs, and is passed every cycle to updateCamera to allow smooth simultaneous camera movement
typedef struct cStates 
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool in;
    bool out;
} cStates;

// Cell Data: The data for the cell, which is paired with cellGraphics to form cellThing
typedef struct cellData
{
    int column;
    int row;
    bool lifeState;
    int aliveNeighbours;
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

// Variables for other functions
extern struct entity* bgTexture;
extern struct mTexture bgTextureM;
extern struct cStates cameraStates;
extern bool quit;

// Functions
transCoords* centreGraphic(int graphicWidth, int graphicHeight);
entity* createEntity(void *selectedThing, int selectedType, int posX, int posY, int initW, int initH, entity** list, int zoom, int* counter);
void entityRender(entity *selectedEntity, entity *camera);
void updateCamera(entity *camera, entity** list);
cStates* updateCameraStates(bool keyState, int keyChoice);
entity** initialiseCellGrid(entity** selectCellGrid, int selectLevelWidth, int selectLevelHeight, entity** list, int* counter, int* addressList, int* addressCount);

#endif // GAME_H