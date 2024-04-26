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
kStates keyStates = {false, false, false, false};
bool quit = false;

// Returns all cells to "not changed" so that they can be flipped by simulation in next tick, and recalculates how many alive neighbours each cell has.
void calculate(entity** grid)
{
	for (int i = 0; i < 1225; i++)
	{
		entity* cellPtr = grid[i];
		cellThing* thingPtr = (cellThing*)grid[i]->thing;
		cellData* dataPtr = thingPtr->data;
		for (int j = 0; j < 1225; j++)
		{
			cellThing* currentThingPtr = (cellThing*)grid[j]->thing;
			cellData* currentDataPtr = currentThingPtr->data;
			if 	((((currentDataPtr->row == (dataPtr->row - 1)) && (currentDataPtr->column == dataPtr->column))	// North
				||((currentDataPtr->row == (dataPtr->row + 1)) && (currentDataPtr->column == dataPtr->column)) 	// South
				||((currentDataPtr->column == (dataPtr->column - 1)) && (currentDataPtr->row == dataPtr->row))	// West
				||((currentDataPtr->column == (dataPtr->column + 1)) && (currentDataPtr->row == dataPtr->row))	// East
				||((currentDataPtr->row == (dataPtr->row - 1)) && (currentDataPtr->column == (dataPtr->column + 1))) // North-east
				||((currentDataPtr->row == (dataPtr->row + 1)) && (currentDataPtr->column == (dataPtr->column + 1))) // South-east
				||((currentDataPtr->row == (dataPtr->row - 1)) && (currentDataPtr->column == (dataPtr->column - 1))) // North-west
				||((currentDataPtr->row == (dataPtr->row + 1)) && (currentDataPtr->column == (dataPtr->column - 1))) // South-west
				) && (currentDataPtr->lifeState))
				{
					dataPtr->aliveNeighbours++;
				}
		}
	}
}

// Once started, will simulate Conway's game determined by the cells activated before start.
void simulate(entity** grid)
{
	for (int i = 0; i < 1225; i++)
	{
		cellThing* thingPtr = (cellThing*)grid[i]->thing;
		cellData* dataPtr = thingPtr->data;

		// Alter cell depending on conditions of the game
		if (dataPtr->lifeState == true)
		{
			if (dataPtr->aliveNeighbours <= 1 || dataPtr->aliveNeighbours >= 4)
			{
				dataPtr->lifeState = false;
			}
			continue;
		} else if (dataPtr->lifeState == false) {
			if (dataPtr->aliveNeighbours == 3)
			{
				dataPtr->lifeState = true;
			}
			continue;
		}
	}
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
	list[*counter] = newEntity;
	*counter = *counter + 1;
	return newEntity;
}

// Creates a singular cell
entity* createCell(int selectColumn, int selectRow, entity** list, int* counter, void*** addressList, int* addressCount, int gridID)
{
	SDL_Rect* newGraphics = createCellGraphics((selectColumn) * CELL_X_PADDING, (selectRow) * CELL_Y_PADDING, CELL_WIDTH, CELL_HEIGHT, addressList, addressCount);
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
	newData->alreadyChanged = false;
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
			entity* nextCell = createCell(i, j, list, counter, addressList, addressCount, k);
			selectCellGrid[k] = nextCell;
			k = k + 1;
		}
	}
	return selectCellGrid;
}

// Modifies an entity's coordinates relative to the coordinates of the camera
void entityUpdate(entity *selectedEntity, entity *camera)
{
	// Determines based on keypress how to perform maths so as to move world correctly relative camera,
	if (keyStates.up) {
		selectedEntity->y = selectedEntity->initY + 10;
		selectedEntity->x = selectedEntity->initX;
	} else if (keyStates.down) {
		selectedEntity->y = selectedEntity->initY - 10;
		selectedEntity->x = selectedEntity->initX;
	} else if (keyStates.left) {
		selectedEntity->x = selectedEntity->initX + 10;
		selectedEntity->y = selectedEntity->initY;
	} else if (keyStates.right) {
		selectedEntity->x = selectedEntity->initX - 10;
		selectedEntity->y = selectedEntity->initY;
	} else {
		return;
	}

	// Makes sure to update internal values of cells so that the next round will work.
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

// Updates game depending on movement keys using entityRender
void updateGame(entity* camera, entity** list)
{
	int i = 0;
	do
	{
		if (list[i] == NULL)
		{
			break;
		} else if (!(list[i]->form == T_CAMERA)) {
			entityUpdate(list[i], camera);
		}
		i = i + 1;
	} while (list[i]->thing != NULL);
	return;
}

// Updates keyStates whenever a valid key is pressed (whether on keyboard or mouse).
void updateKeyStates(bool keyState, int keyChoice, entity** grid)
{
	switch(keyChoice)
	{
		case SDLK_UP:
			keyStates.up = keyState;
			break;
		case SDLK_DOWN:
			keyStates.down = keyState;
			break;
		case SDLK_LEFT:
			keyStates.left = keyState;
			break;
		case SDLK_RIGHT:
			keyStates.right = keyState;
			break;
		case SDLK_SPACE:
			if (keyStates.start != true)
			{
				keyStates.start = keyState;
			}
	}
	return;
}

// Flips cell's life state if it is clicked by mouse.
void updateCellStates(bool state, int posX, int posY, entity** grid)
{
	// Checks wheter mouse button is pressed, because only when the mouse is clicked do we need to check to see if a cell must be flipped.
	if (state)
	{
		// Loops through cell grid and checks whether the cursor's coordinates are in the range of the cell
		for (int i = 0; i < 1225; i++)
		{
			// if the mouse is between its origin coordinate and said coordinate multiplied by its width/height
			if ((posX > grid[i]->x) && (posX < (grid[i]->x + grid[i]->w)))
			{
				if ((posY > grid[i]->y) && (posY < (grid[i]->y + grid[i]->h)))
				{
					cellThing* thingPtr = (cellThing*)grid[i]->thing;
					cellData* dataPtr = thingPtr->data;
					if (dataPtr->lifeState)
					{
						dataPtr->lifeState = false;
					} else { dataPtr->lifeState = true; }
				}
			}
		}	
	}
	return;
}