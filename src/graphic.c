#include <stdio.h>
#include <SDL2/SDL.h>

#include "error.h"
#include "game.h"
#include "main.h"

// Declare global variables defined in header
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currentSurface = NULL;
SDL_Renderer* renderer = NULL;

// Creates the graphics for a single cell
SDL_Rect* createCellGraphics(int selectX, int selectY, int selectW, int selectH)
{
	SDL_Rect* newCellGraphics = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (newCellGraphics == NULL)
	{
		errorHandle(E_CELL_GRAPHICS_MEM);
		return;
	}
	newCellGraphics->x = selectX;
	newCellGraphics->y = selectY;
	newCellGraphics->w = selectW;
	newCellGraphics->h = selectH;
	return &newCellGraphics;
}

// Loads surface, but can't be used with textures/rendering stuff
SDL_Surface* loadSurface(char* path)
{
	// Final optomised image
	SDL_Surface* actualSurface = NULL;
	// Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL)
	{
		errorHandle(E_SDL_LOAD_IMAGE, path, SDL_GetError);
	}
	else {
		// Convert surface, remove old one
		actualSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
		if (actualSurface == NULL)
		{
			errorHandle(E_SDL_OPTOMISE_BMP, path, SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return actualSurface;
}

// Loads textures
mTexture loadTexture(char *path, int destTexX, int destTexY, int destTexW, int destTexH)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if (loadedSurface == NULL)
	{
		errorHandle(E_SDL_LOAD_IMAGE);
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			errorHandle(E_SDL_CREATE_TEXTURE_IMAGE, path, SDL_GetError());
		}
		else {
			SDL_FreeSurface(loadedSurface);
			SDL_Rect dRect = {destTexX, destTexY, destTexW, destTexH};
			mTexture actualTexture = {newTexture, dRect};
			return actualTexture;
		}
	}
}

// Get SDL_Texture from entity containing mTexture
SDL_Texture *getTexture(entity *selectedEntity)
{
	mTexture *ptr = (mTexture*)selectedEntity->thing;
	return ptr->texture;
}

// Get destinationRect from entity containing mTexture
SDL_Rect getDestinationRect(entity *selectedEntity)
{
	mTexture *ptr = (mTexture*)selectedEntity->thing;
	return ptr->destinationRect;
}

// Loads media
int loadMedia(entity** list, int* counter)
{
	transCoords* transform = centreGraphic(640, 480);
	bgTextureM = loadTexture("../img/bg.bmp", transform->x, transform->y, 640, 480);
	bgTexture = createEntity(&bgTextureM, T_MTEXTURE, transform->x, transform->y, 640, 480, list, 1.0, counter);
	return 0;
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