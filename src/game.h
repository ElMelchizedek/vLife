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

// Type macros
#define T_CAMERA    0
#define T_SDL_RECT  1
#define T_MTEXTURE  2

// Zoom macros
#define ZOOM_IN true
#define ZOOM_OUT false

// Variables for other functions
extern struct entity bgTexture;
extern struct mTexture bgTextureM;
extern bool quit;

// Functions
transCoords *centreGraphic(int graphicWidth, int graphicHeight);
entity createEntity(void *selectedThing, int selectedType, int posX, int posY, int initW, int initH, entity** list, int zoom, int* counter);
void entityRender(entity *selectedEntity, entity *camera, int keyChoice);
void updateCamera(entity *camera, int keyChoice, entity** list);

#endif // GAME_H