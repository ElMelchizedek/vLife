#include <stdio.h>
#include <SDL2/SDL.h>

#include "error.h"
#include "game.h"
#include "main.h"

// Declare global variables defined in header
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Creates the graphics for a single cell
SDL_Rect* createCellGraphics(int selectX, int selectY, int selectW, int selectH, void*** addressList, int* addressCount)
{
	SDL_Rect* newCellGraphics = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (newCellGraphics == NULL)
	{
		errorHandle(E_MEM, "cellGraphics");
	}
	newCellGraphics->x = selectX;
	newCellGraphics->y = selectY;
	newCellGraphics->w = selectW;
	newCellGraphics->h = selectH;
	saveAddress(addressList, addressCount, (void*)newCellGraphics);
	return newCellGraphics;
}

// Initialise SDL in programme
int graphicInit()
{
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		errorHandle(E_SDL_INIT, SDL_GetError());
		return 1;
	}
	else {
		// Create window
		window = SDL_CreateWindow("Verl's Game of Life", 
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			errorHandle(E_SDL_WINDOW_CREATE, SDL_GetError());
			return 1;
		}
		else {
			// Create renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				errorHandle(E_SDL_RENDERER_CREATE);
				return 1;
			}
			else {
				// Initialise renderer colour
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return 0;
}