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
const int LEVEL_WIDTH = 1000;
const int LEVEL_HEIGHT = 1000;

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


int main (int argc, char* argv[])
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
		for (int i = 0; i < 10; i++)
		{
			mainEntityList[i] = malloc(sizeof(entity));
			mainEntityList[i]->thing = NULL;
			mainEntityList[i]->form = 0;
			mainEntityList[i]->x = 0;
			mainEntityList[i]->y = 0;
			mainEntityList[i]->initX = 0;
			mainEntityList[i]->initY = 0;
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

			// Set up camera
			SDL_Rect cameraRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			entity camera = createEntity(&cameraRect, T_CAMERA, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mainEntityList, 1.0, &mainEntityListCount);

			// Variables required to be declared before main loop due to memory allocation

			// transCoords* rectTransform = centreGraphic(50, 50);
			// SDL_Rect outlineRect;
			// outlineRect.x = rectTransform->x;
			// outlineRect.y = rectTransform->y;
			// outlineRect.w = rectTransform->w;
			// outlineRect.h = rectTransform->h;
			// entity redRect = createEntity(&outlineRect, T_SDL_RECT, outlineRect.x, outlineRect.y, outlineRect.w, outlineRect.h, mainEntityList, 1.0, &mainEntityListCount);



			// Main loop
			while (!quit)
			{
				// Event handler loop
				while(SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					} else if (e.type == SDL_KEYDOWN) {
						updateCameraStates(true, e.key.keysym.sym);
					} else if (e.type == SDL_KEYUP) {
						updateCameraStates(false, e.key.keysym.sym);
					} else if (e.type == SDL_MOUSEBUTTONDOWN) {
						updateCameraStates(false, SDL_MOUSEBUTTONDOWN);
					}
				}
				updateCamera(&camera, mainEntityList);
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				// SDL_Rect* temp = (SDL_Rect*)redRect.thing;
				// SDL_RenderClear(renderer);
				// SDL_Rect tempRect = getDestinationRect(&bgTexture);
				// const SDL_Rect* tempRectPtr = &tempRect;
				// SDL_RenderCopy(renderer, getTexture(&bgTexture), NULL, tempRectPtr);
				// SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				// SDL_RenderDrawRect(renderer, &outlineRect);
				SDL_RenderPresent(renderer);
				// GPU usage skyrockets without this
				SDL_Delay(17);
			}
		}
	}

	// Close programme
	end();
	return 0;
}
