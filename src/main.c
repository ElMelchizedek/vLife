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
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

// Quit programme
void end()
{
	// Deallocate surface, destroy window, etc, & quit
	SDL_Texture *actualBGTexture = getTexture(&bgTexture);
	SDL_DestroyTexture(actualBGTexture);
	actualBGTexture = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}


int main (int argc, char *argv[])
{
	// Initialise 
	if (graphicInit())
	{
		errorHandle(E_INIT);
	}
	else {
		// Initialise entity list with placeholder values
		entity *mainEntityList[10];
		int mainEntityListCount = 0;
		for (int i = 0; i < 2; ++i)
		{
			mainEntityList[i] = NULL;
		}

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

			// Set up camerac
			int *cameraTransform = centreGraphic(SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			free(cameraTransform);

			// Variables required to be declared before main loop due to memory allocation
			int* rectTransform = centreGraphic((SCREEN_WIDTH / 6), (SCREEN_HEIGHT / 6));
			SDL_Rect fillRect = {rectTransform[0], rectTransform[1], rectTransform[2], rectTransform[3]};
			entity redRect = createEntity(&fillRect, T_SDL_RECT, fillRect.x, fillRect.y, mainEntityList, &mainEntityListCount);
			free(rectTransform);
			// Main loop
			while (!quit)
			{
				// Event handler loop
				while(SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						// Free things made in main that have memory allocations
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						switch(e.key.keysym.sym)
						{
							case (SDLK_UP):
								{
									updateCamera(&camera, K_UP, mainEntityList);

								}
							case (SDLK_DOWN):
								{
									updateCamera(&camera, K_DOWN, mainEntityList);
								}
							default:
								break;
						}
					}
				}
				SDL_Rect* temp = (SDL_Rect*)redRect.thing;
				SDL_RenderClear(renderer);
				SDL_Rect tempRect = getDestinationRect(&bgTexture);
				const SDL_Rect* tempRectPtr = &tempRect;
				SDL_RenderCopy(renderer, getTexture(&bgTexture), NULL, tempRectPtr);
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &fillRect);
				SDL_RenderPresent(renderer);
			}
		}
	}

	// Close programme
	end();
	return 0;
}
