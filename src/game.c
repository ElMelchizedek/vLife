// Standard library & SDL
#include <stdio.h>
#include <stdbool.h>
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
int *centreGraphic(int graphicWidth, int graphicHeight)
{
	int *transformNums = malloc(16);
		transformNums[0] = ((SCREEN_WIDTH / 2) - (graphicWidth / 2));
		transformNums[1] = ((SCREEN_HEIGHT / 2) - (graphicHeight / 2));
		transformNums[2] = graphicWidth;
		transformNums[3] = graphicHeight;
	return transformNums;
}

// Creates entity
entity createEntity(void *selectedThing, int selectedType, int posX, int posY, entity **list, int *counter)
{
	entity *newEntity = (entity*)malloc(sizeof(entity));
	newEntity->thing = selectedThing;
	newEntity->form = selectedType;
	newEntity->x = posX;
	newEntity->y = posY;
	newEntity->initX = posX;
	newEntity->initY = posY;
	list[*counter] = newEntity;
	*counter = *counter + 1;
	return *newEntity;
}

// Modifies an entity's coordinates relative to the coordinates of the camera
void entityRender(entity *selectedEntity, SDL_Rect *camera, int keyChoice)
{
	// Determines based on keypress how to perform maths so as to move world correctly relative camera
	switch (keyChoice)
	{
		case K_UP:
			selectedEntity->y = selectedEntity->initY - camera->y;
			break;
		case K_DOWN:
			selectedEntity->y = selectedEntity->initY + camera->y;
			break;
		case K_LEFT:
			selectedEntity->x = selectedEntity->initX - camera->x;
			break;
		case K_RIGHT:
			selectedEntity->x = selectedEntity->initX + camera->x;
			break;
	}
	// change actual coords per data form
	if (selectedEntity->form == 0)
	{
		SDL_Rect *ptr = (SDL_Rect*)selectedEntity->thing;
		ptr->x = selectedEntity->x;
		ptr->y = selectedEntity->y;
		
	} else if (selectedEntity->form == 1) {
		mTexture *ptr = (mTexture*)selectedEntity->thing;
		ptr->destinationRect.x = selectedEntity->x;
		ptr->destinationRect.y = selectedEntity->y;
	} else {
		errorHandle(E_UNSUPPORTED_ENTITY_THING_TYPE, selectedEntity->form);
	}
	return;
}

// Updates camera position depending on movement keys, then calls entityRender
void updateCamera(SDL_Rect *camera, int keyChoice, entity **list)
{
	switch(keyChoice)
	{
		case K_UP:
			(camera->y) = (camera->y) - 1;
			break;
		case K_DOWN:
			(camera->y) = (camera->y) + 1;
			break;
		case K_LEFT:
			(camera->x) = (camera->x) - 1;
			break;
		case K_RIGHT:
			(camera->x) = (camera->x) + 1;
			break;
	}
	int i = 0;
	do
	{
		if (list[i] == NULL)
		{
			break;
		} else {
			entityRender(list[i], camera, keyChoice);
		}
		i = i + 1;
	} while (i < (int)(sizeof(list)/sizeof(list[0])));
	return;
}