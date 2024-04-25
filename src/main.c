// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <SDL2/SDL.h>

// Own code
#include "error.h"
#include "game.h"
#include "graphic.h"

// Constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int LEVEL_WIDTH = 350;
const int LEVEL_HEIGHT = 350; 

// Quit programme
void end()
{
	// Deallocate surface, destroy window, etc, & quit
	if (bgTexture != NULL)
	{
		SDL_Texture *actualBGTexture = getTexture(bgTexture);
		SDL_DestroyTexture(actualBGTexture);
		actualBGTexture = NULL;
	}
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}

void saveAddress(void*** addressList, int* addressCount, void* selectThing)
{
	*addressCount = *addressCount + 1;
	*addressList = (void**)realloc(*addressList, *addressCount * sizeof(void*));
	(*addressList)[*addressCount - 1] = selectThing;
	return;
}

int main (int argc, char* argv[])
{
	// Initialise 
	if (graphicInit())
	{
		errorHandle(E_INIT);
	}
	else {
		// Array of pointers to memory allocated variables, to be freed on quit
		void** addressList = (void**)malloc(1 * sizeof(void*));
		if (addressList == NULL)
		{
			errorHandle(E_MEM, "addressList");
		}
		//addressList[0] = (void*)malloc(sizeof(void));
		int addressListCount = 0;
		// Declare entity list counter
		int mainEntityListCount = 0;
		// Allocate memory for entity list, then initialise its pointer elements
		entity** mainEntityList = (entity**)malloc(10010 * sizeof(entity*));
		if (mainEntityList == NULL)
		{
			errorHandle(E_MEM, "mainEntityList");
		}
		for (int i = 0; i < 10010; i++)
		{
			mainEntityList[i] = malloc(sizeof(entity));
			mainEntityList[i]->thing = NULL;
			mainEntityList[i]->form = 0;
			mainEntityList[i]->x = 0;
			mainEntityList[i]->y = 0;
			mainEntityList[i]->initX = 0;
			mainEntityList[i]->initY = 0;
		}
		saveAddress(&addressList, &addressListCount, (void*)mainEntityList);
		// Allocate memeory for cell grid, then initialise its pointer elements
		entity** cellGrid = (entity**)malloc(10000 * sizeof(entity*));
		if (cellGrid == NULL)
		{
			errorHandle(E_MEM, "cellGrid");
		}
		cellGrid = initialiseCellGrid(cellGrid, LEVEL_WIDTH, LEVEL_HEIGHT, mainEntityList, &mainEntityListCount, &addressList, &addressListCount);
		saveAddress(&addressList, &addressListCount, (void*)&cellGrid);
		// Load media
		if (loadMedia(mainEntityList, &mainEntityListCount))
		{
			errorHandle(E_LOAD_MEDIA);
		}
		else {
			SDL_Event e;

			// Set viewport
			SDL_Rect mainViewport;
			mainViewport.x = 0;
			mainViewport.y = 0;
			mainViewport.w = SCREEN_WIDTH;
			mainViewport.h = SCREEN_HEIGHT;
			SDL_RenderSetViewport(renderer, &mainViewport);

			// Set up camera
			SDL_Rect cameraRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			entity* camera = createEntity(&cameraRect, T_CAMERA, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mainEntityList, &mainEntityListCount);

			// Main loop
			while (!quit)
			{
				// Event handler loop
				while(SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						for (int i = 0; i <= addressListCount; i++)
						{
							free(addressList[i]);
						}
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						updateCameraStates(true, e.key.keysym.sym);
					} else if (e.type == SDL_KEYUP) {
						updateCameraStates(false, e.key.keysym.sym);
					} else if (e.type == SDL_MOUSEBUTTONDOWN) {
						updateCameraStates(false, SDL_MOUSEBUTTONDOWN);
					}
				}
				updateCamera(camera, mainEntityList, &cellGrid);
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				
				for (int i = 0; i < ((LEVEL_WIDTH / 10) * (LEVEL_HEIGHT / 10)); i++)
				{
					entity* cellPtr = cellGrid[i];
					cellThing* thingPtr = (cellThing*)cellPtr->thing;
					SDL_Rect* rectPtr = thingPtr->graphics;
					SDL_RenderDrawRect(renderer, rectPtr); 
				}
				SDL_RenderPresent(renderer);
				SDL_Delay(17);
			}
		}
	}

	// Close programme
	end();
	return 0;
}
