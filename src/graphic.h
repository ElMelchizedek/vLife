#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include "game.h"

// Foward declerations of structs for types defined in game.h to break circular dependency issue.
struct entity;

// SDL global variables
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Functions
SDL_Rect* createCellGraphics(int selectX, int selectY, int selectW, int selectH, void*** addressList, int* addressCount);
int graphicInit();

#endif