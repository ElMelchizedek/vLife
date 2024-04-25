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
cStates cameraStates = {false, false, false, false};
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
entity *createEntity(void *selectedThing, int selectedType, int posX, int posY, int initW, int initH, entity** list, int* counter)
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
	newEntity->changed = false;
	list[*counter] = newEntity;
	*counter = *counter + 1;
	return newEntity;
}

// Creates a singular cell
entity* createCell(int selectColumn, int selectRow, entity** list, int* counter, void*** addressList, int* addressCount, int gridID)
{
	SDL_Rect* newGraphics = createCellGraphics((selectRow) * 20, (selectColumn) * 20, 20, 20, addressList, addressCount);
	saveAddress(addressList, addressCount, (void*)newGraphics);
	cellData* newData = (cellData*)malloc(sizeof(cellData));
	if (newData == NULL)
	{
		errorHandle(E_MEM, "cellData");
	}
	newData->column = selectColumn;
	newData->row = selectRow;
	newData->lifeState = false;
	newData->aliveNeighbours = 0;
	saveAddress(addressList, addressCount, (void*)newData);

	cellThing* newThing = (cellThing*)malloc(sizeof(cellThing));
	if (newThing == NULL)
	{
		errorHandle(E_MEM, "cellThing");
	}
	newThing->data = newData;
	newThing->graphics = newGraphics;
	saveAddress(addressList, addressCount, (void*)newThing);

	entity* newCell = createEntity(newThing,
									T_CELL,
									newGraphics->x,
									newGraphics->y,
									newGraphics->w,
									newGraphics->h,
									list,
									counter);
	if (newCell == NULL)
	{
		errorHandle(E_MEM, "entity,cell");
	}
	saveAddress(addressList, addressCount, (void*)newCell);
	return newCell;
}

// Initialises the grid of cells to be used by the game
entity** initialiseCellGrid(entity** selectCellGrid, int selectLevelWidth, int selectLevelHeight, entity** list, int* counter, void*** addressList, int* addressCount)
{
	// Declares the amount of cells to be, based on the level size and how big each cell will be
	float gridWidth = ((float)selectLevelWidth / 10);
	float gridHeight = ((float)selectLevelHeight / 10);
	// If we don't get a round number, we'll have issues creating cells and so should abort
	if (fmod(gridWidth, 1) || fmod(gridWidth, 1))
	{
		errorHandle(E_GRID_FLOAT, selectLevelWidth, selectLevelHeight);
	}
	// Loop to generate each cell to its starting condition
	int k = 0;
	for (int i = 0; i < gridHeight; ++i)
	{
		for (int j = 0; j < gridWidth; ++j)
		{
			printf("k%d\n", k);
			entity* nextCell = createCell(i, j, list, counter, addressList, addressCount, k);
			selectCellGrid[k] = nextCell;
			k = k + 1;
		}
	}
	return selectCellGrid;
}

// Modifies an entity's coordinates relative to the coordinates of the camera
void entityRender(entity *selectedEntity, entity *camera)
{
	// Determines based on keypress how to perform maths so as to move world correctly relative camera
	if (cameraStates.up) {
		selectedEntity->y = selectedEntity->initY + 10;
		selectedEntity->x = selectedEntity->initX;
	} else if (cameraStates.down) {
		selectedEntity->y = selectedEntity->initY - 10;
		selectedEntity->x = selectedEntity->initX;
	} else if (cameraStates.left) {
		selectedEntity->x = selectedEntity->initX + 10;
		selectedEntity->y = selectedEntity->initY;
	} else if (cameraStates.right) {
		selectedEntity->x = selectedEntity->initX - 10;
		selectedEntity->y = selectedEntity->initY;
	} else {
		return;
	}

	if (selectedEntity->form == T_CELL)
	{
		cellThing* thingPtr = (cellThing*)selectedEntity->thing;
		SDL_Rect* rectPtr = thingPtr->graphics;
		rectPtr->x = selectedEntity->x;
		rectPtr->y = selectedEntity->y;
		selectedEntity->initX = rectPtr->x;
		selectedEntity->initY = rectPtr->y;
	}
	return;
}

// Updates camera position depending on movement keys, then calls entityRender
void updateCamera(entity* camera, entity** list, entity*** cellGrid)
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
	} else {
		return;
	}
	int i = 0;
	do
	{
		//(entity*) list;
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
void updateCameraStates(bool keyState, int keyChoice)
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
	}
}