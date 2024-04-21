#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "graphic.h"

// Custom types from structs in corresponding source file
// Declare structs
typedef struct entity
{
    void *thing;
    int form;
    int x;
    int y;
    int initX;
    int initY;
} entity;

typedef struct mTexture 
{
    SDL_Texture *texture;
    SDL_Rect destinationRect;
} mTexture;

// Type macros
#define T_SDL_RECT 0
#define T_MTEXTURE 1

// Key macros
#define K_UP 0
#define K_DOWN 1
#define K_LEFT 2
#define K_RIGHT 3

// Variables for other functions
extern struct entity bgTexture;
extern struct mTexture bgTextureM;
extern bool quit;

// Functions
int *centreGraphic(int graphicWidth, int graphicHeight);
entity createEntity(void *selectedThing, int selectedType, int posX, int posY, entity **list, int *counter);
void entityRender(entity *selectedEntity, SDL_Rect *camera, int keyChoice);
void updateCamera(SDL_Rect *camera, int keyChoice, entity **list);

#endif // GAME_H