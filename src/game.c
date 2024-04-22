// Standard library & SDL
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

// Own code
#include "main.h"
#include "error.h"
#include "game.h"

// Define global variables declared in header
entity bgTexture;
mTexture bgTextureM;
bool quit = false;

// Returns coords + width and height for a centered graphic
transCoords* centreGraphic(int graphicWidth, int graphicHeight)
{
	transCoords* centeredTransCoords = (transCoords*)malloc(sizeof(transCoords));
	centeredTransCoords->x = ((SCREEN_WIDTH / 2) - (graphicWidth / 2));
	centeredTransCoords->y = ((SCREEN_HEIGHT / 2) - (graphicHeight / 2));
	centeredTransCoords->w = graphicWidth;
	centeredTransCoords->h = graphicHeight;
	return centeredTransCoords;
}

// Creates entity
entity createEntity(void *selectedThing, int selectedType, int posX, int posY, int initW, int initH, entity** list, int zoom, int* counter)
{
	entity *newEntity = (entity*)malloc(sizeof(entity));
	newEntity->thing = selectedThing;
	newEntity->form = selectedType;
	newEntity->x = posX;
	newEntity->y = posY;
	newEntity->initX = posX;
	newEntity->initY = posY;
	newEntity->zoom = zoom;
	list[*counter] = newEntity;
	*counter = *counter + 1;
	return *newEntity;
}

// Modifies an entity's coordinates relative to the coordinates of the camera
void entityRender(entity *selectedEntity, entity *camera, Sint32 keyChoice)
{
	// zoomKey: whether zoom performed at all
	// zoomForm: whether zoom is in or out
	// zoomMod: modifier to be applied to current zoom of entity
	bool zoomKey = false;
	bool zoomForm = NULL;
	int zoomMod = NULL;

	// Determines based on keypress how to perform maths so as to move world correctly relative camera
	switch (keyChoice)
	{
		case SDLK_UP:
			selectedEntity->y = selectedEntity->initY + camera->y;
			break;
		case SDLK_DOWN:
			selectedEntity->y = selectedEntity->initY + camera->y;
			break;
		case SDLK_LEFT:
			selectedEntity->x = selectedEntity->initX + camera->x;
			break;
		case SDLK_RIGHT:
			selectedEntity->x = selectedEntity->initX + camera->x;
			break;
		case SDLK_EQUALS:
			zoomKey = true;
			zoomForm = ZOOM_IN;
			zoomMod = 25;
			break;
		case SDLK_MINUS:
			zoomKey = true;
			zoomForm = ZOOM_OUT;
			zoomMod = -25;
			break;
	}

	// In case of zoom key pressed, perform these actions regardless of whether in or out
	if (zoomKey)
	{
		selectedEntity->zoom = camera->zoom;
		//printf("%f\n", selectedEntity->zoom);
	}

	// change actual coords/transform per data form
	if (selectedEntity->form == T_SDL_RECT)
	{
		SDL_Rect* actualRectPtr = (SDL_Rect*)selectedEntity->thing;
		actualRectPtr->x = selectedEntity->x;
		actualRectPtr->y = selectedEntity->y;
		if (zoomKey)
		{
			actualRectPtr->w = actualRectPtr->w + zoomMod;
			actualRectPtr->h = actualRectPtr->h + zoomMod;
		}
	} else if (selectedEntity->form == T_MTEXTURE) {
		mTexture* mTexPtr = (mTexture*)selectedEntity->thing;
		mTexPtr->destinationRect.x = selectedEntity->x;
		mTexPtr->destinationRect.y = selectedEntity->y;
		if (zoomKey)
		{
			mTexPtr->destinationRect.w = mTexPtr->destinationRect.w + zoomMod;
			mTexPtr->destinationRect.h = mTexPtr->destinationRect.h + zoomMod;
		}
	} else {
		errorHandle(E_UNSUPPORTED_ENTITY_THING_TYPE, selectedEntity->form);
	}
	return;
}

// Updates camera position depending on movement keys, then calls entityRender
void updateCamera(entity* camera, int keyChoice, entity** list)
{
	switch(keyChoice)
	{
		case SDLK_UP:
			camera->y = (camera->y - 10);
			break;
		case SDLK_DOWN:
			camera->y = (camera->y + 10);
			break;
		case SDLK_LEFT:
			camera->x = (camera->x - 10);
			break;
		case SDLK_RIGHT:
			camera->x = (camera->x + 10);
			break;
		case SDLK_EQUALS:
			if (camera->zoom < 2.0)
			{
				camera->zoom = (camera->zoom + 0.1);
			}
			else {
				keyChoice = NULL;
			}
			break;
		case SDLK_MINUS:
			if (camera->zoom > 0.1)
			{
				camera->zoom = (camera->zoom - 0.1);
			}
			else {
				keyChoice = NULL;
			}
			break;
	}
	int i = 0;
	do
	{
		(entity*) list;
		if (list[i] == NULL)
		{
			break;
		} else if (!(list[i]->form == T_CAMERA)) {
			entityRender(list[i], camera, keyChoice);
		}
		i = i + 1;
	} while (list[i]->thing != NULL);
	return;
}