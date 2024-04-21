#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include "game.h"

// Foward declerations of structs for types defined in game.h to break circular dependency issue.
struct entity;
struct mTexture;

// SDL global variables
extern SDL_Window* window;
extern SDL_Surface* screenSurface;
extern SDL_Surface* currentSurface;
extern SDL_Renderer* renderer;

// Functions
SDL_Surface* loadSurface(char* path);
struct mTexture loadTexture(char* path, int destTexX, int destTexy, int desTexW, int destTexH);
SDL_Texture* getTexture(struct entity* selectedEntity);
SDL_Rect getDestinationRect(struct entity *selectedEntity);
int loadMedia(struct entity** list, int* counter);
int graphicInit();

#endif