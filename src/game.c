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
entity* bgTexture = NULL;
mTexture bgTextureM;
cStates cameraStates = {false, false, false, false, false, false};
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
entity *createEntity(void *selectedThing, int selectedType, int posX, int posY, int initW, int initH, entity** list, int zoom, int* counter)
{
	entity *newEntity = (entity*)malloc(sizeof(entity));
	if (newEntity == NULL)
	{
		errorHandle(E_MEM, "entity");
	}
	newEntity->thing = selectedThing;
	newEntity->form = selectedType;
	newEntity->x = posX;
	newEntity->y = posY;
	newEntity->w = initW;
	newEntity->h = initH;
	newEntity->initX = posX;
	newEntity->initY = posY;
	newEntity->zoom = zoom;
	list[*counter] = newEntity;
	*counter = *counter + 1;
	return newEntity;
}

// Creates a singular cell
entity* createCell(int selectColumn, int selectRow, entity** list, int* counter, int* addressList, int* addressCount)
{
	SDL_Rect* newGraphics = createCellGraphics((10 * selectRow), (10 * selectColumn), 10, 10, addressList, addressCount);
	saveAddress(addressList, addressCount, (uintptr_t)newGraphics);
	cellData* newData = (cellData*)malloc(sizeof(cellData));
	if (newData == NULL)
	{
		errorHandle(E_MEM, "cellData");
	}
	newData->column = selectColumn;
	newData->row = selectRow;
	newData->lifeState = false;
	newData->aliveNeighbours = 0;
	saveAddress(addressList, addressCount, (uintptr_t)newData);

	cellThing* newThing = (cellThing*)malloc(sizeof(cellThing));
	if (newThing == NULL)
	{
		errorHandle(E_MEM, "cellThing");
	}
	newThing->data = newData;
	newThing->graphics = newGraphics;
	saveAddress(addressList, addressCount, (uintptr_t)newThing);

	entity* newCell = createEntity(newThing,
									T_CELL,
									newGraphics->x,
									newGraphics->y,
									newGraphics->w,
									newGraphics->h,
									list,
									1.0,
									counter);
	//entity* newCellPtr = &newCell;
	if (newCell == NULL)
	{
		errorHandle(E_MEM, "entity,cell");
	}
	saveAddress(addressList, addressCount, (uintptr_t)newCell);
	return newCell;
}

// Initialises the grid of cells to be used by the game
entity** initialiseCellGrid(entity** selectCellGrid, int selectLevelWidth, int selectLevelHeight, entity** list, int* counter,
							int* addressList, int* addressCount)
{
	// Declares the amount of cells to be, based on the level size and how big each cell will be
	float gridWidth = ((float)selectLevelWidth / 10);
	float gridHeight = ((float)selectLevelHeight / 10);
	// If we don't get a round number, we'll have issues creating cells and so should abort
	if (fmod(gridWidth, 1) || fmod(gridWidth,1))
	{
		errorHandle(E_GRID_FLOAT, selectLevelWidth, selectLevelHeight);
	}
	// Loop to generate each cell to its starting condition
	int k = 0;
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			entity* nextCell = createCell(i, j, list, counter, addressList, addressCount);
			selectCellGrid[k] = nextCell;
			k = k + 1;
		}
	}
	return selectCellGrid;
}

// Modifies an entity's coordinates relative to the coordinates of the camera
void entityRender(entity *selectedEntity, entity *camera)
{
	// zoomKey: whether zoom performed at all
	// zoomForm: whether zoom is in or out
	// zoomMod: modifier to be applied to current zoom of entity
	bool zoomKey = false;
	bool zoomForm;
	int zoomMod;

	// Determines based on keypress how to perform maths so as to move world correctly relative camera
	if (cameraStates.up) {
		selectedEntity->y = selectedEntity->initY - 10;
		selectedEntity->x = selectedEntity->initX;
	} else if (cameraStates.down) {
		selectedEntity->y = selectedEntity->initY + 10;
		selectedEntity->x = selectedEntity->initX;
	} else if (cameraStates.left) {
		selectedEntity->x = selectedEntity->initX - 10;
		selectedEntity->y = selectedEntity->initY;
	} else if (cameraStates.right) {
		selectedEntity->x = selectedEntity->initX + 10;
		selectedEntity->y = selectedEntity->initY;
	} else if (cameraStates.in) {
		zoomKey = true;
		zoomForm = ZOOM_IN;
		zoomMod = 25;
	} else if (cameraStates.out) {
		zoomKey = true;
		zoomForm = ZOOM_OUT;
		zoomMod = -25;
	} else {
		return;
	}

	// In case of zoom key pressed, perform these actions regardless of whether in or out
	if (zoomKey)
	{
		selectedEntity->zoom = camera->zoom;
	}

	// change actual coords/transform per data form
	if (selectedEntity->form == T_SDL_RECT)
	{
		SDL_Rect* actualRectPtr = (SDL_Rect*)selectedEntity->thing;
		if (!zoomKey) {
			actualRectPtr->x = selectedEntity->x;
			actualRectPtr->y = selectedEntity->y;
		} else {
			int oldWidth = actualRectPtr->w;
			int oldHeight = actualRectPtr->h;
			actualRectPtr->w = (selectedEntity->w * camera->zoom);
			actualRectPtr->h = (selectedEntity->h * camera->zoom);
			actualRectPtr->x = (actualRectPtr->x - ((actualRectPtr->w - oldWidth) / 2));
			actualRectPtr->y = (actualRectPtr->y - ((actualRectPtr->h - oldHeight) / 2));
		}
		selectedEntity->initX = actualRectPtr->x;
		selectedEntity->initY = actualRectPtr->y;
	} else if (selectedEntity->form == T_MTEXTURE) {
		mTexture* mTexPtr = (mTexture*)selectedEntity->thing;
		if (!zoomKey) {
			mTexPtr->destinationRect.x = selectedEntity->x;
			mTexPtr->destinationRect.y = selectedEntity->y;
		} else {
			int oldWidth = mTexPtr->destinationRect.w;
			int oldHeight = mTexPtr->destinationRect.h;
			mTexPtr->destinationRect.w = (selectedEntity->w * camera->zoom);
			mTexPtr->destinationRect.h = (selectedEntity->h * camera->zoom);
			mTexPtr->destinationRect.x = (mTexPtr->destinationRect.x - ((mTexPtr->destinationRect.w - oldWidth) / 2));
			mTexPtr->destinationRect.y = (mTexPtr->destinationRect.y - ((mTexPtr->destinationRect.h - oldHeight) / 2));
		}
		selectedEntity->initX = mTexPtr->destinationRect.x;
		selectedEntity->initY = mTexPtr->destinationRect.y;
	} else {
		errorHandle(E_UNSUPPORTED_ENTITY_THING_TYPE, selectedEntity->form);
	}
	return;
}

// Updates camera position depending on movement keys, then calls entityRender
void updateCamera(entity* camera, entity** list)
{
	if (cameraStates.up)
	{
		camera->y = (camera->y - 10);
	} else if (cameraStates.down) {
		camera->y = (camera->y + 10);
	} else if (cameraStates.left) {
		camera->x = (camera->x - 10);
	} else if (cameraStates.right) {
		camera->x = (camera->x + 10);
	} else if (cameraStates.in) {
		if (camera->zoom < 10.0)
		{
			camera->zoom = (camera->zoom + 0.1);
		}
	} else if (cameraStates.out) {
		if (camera->zoom > 1.0)
		{
			camera->zoom = (camera->zoom - 0.1);
		}
	} else {
		return;
	}
	int i = 0;
	do
	{
		(entity*) list;
		if (list[i] == NULL)
		{
			break;
		} else if (!(list[i]->form == T_CAMERA)) {
			entityRender(list[i], camera);
		}
		i = i + 1;
	} while (list[i]->thing != NULL);
	return;
}

// Updates cameraState whenever a SDL_KEYUP or SDL_KEYDOWN event is performed
cStates *updateCameraStates(bool keyState, int keyChoice)
{
	switch(keyChoice)
	{
		case SDLK_UP:
			cameraStates.up = keyState;
			break;
		case SDLK_DOWN:
			cameraStates.down = keyState;
			break;
		case SDLK_LEFT:
			cameraStates.left = keyState;
			break;
		case SDLK_RIGHT:
			cameraStates.right = keyState;
			break;
		case SDLK_EQUALS:
			cameraStates.in = keyState;
			break;
		case SDLK_MINUS:
			cameraStates.out = keyState;
			break;
	}
}