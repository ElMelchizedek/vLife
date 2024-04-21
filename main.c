#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

// Error macros
#define E_SDL_INIT 0
#define E_SDL_WINDOW_CREATE 1
#define E_SDL_LOAD_IMAGE 2
#define E_INIT 3
#define E_LOAD_MEDIA 4
#define E_FILE_NOT_FOUND 5
#define E_BUFFER_NULL 6
#define E_FILE_READ 7
#define E_SDL_OPTOMISE_BMP 8
#define E_SDL_RENDERER_CREATE 9
#define E_SDL_CREATE_TEXTURE_IMAGE 10
#define E_UNSUPPORTED_ENTITY_THING_TYPE 11
#define E_ENTITY_NULL 12

// Type macros
#define T_SDL_RECT 0
#define T_MTEXTURE 1

// Key macros
#define K_UP 0
#define K_DOWN 1
#define K_LEFT 2
#define K_RIGHT 3

// Key press constants
enum keyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

// Structs for graphical entities in the program
typedef struct graphicEntity
{
	void *thing;
	int form;
	int x;
	int y;
	int initX;
	int initY;
} entity;

typedef struct moveableTexture
{
	SDL_Texture *texture;
	SDL_Rect destinationRect;
} mTexture;


// SDL global variables
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* currentSurface = NULL;
SDL_Renderer* renderer = NULL;

// Other global variables
entity bgTexture;
mTexture bgTextureM;
bool quit = false;

// Handles errors and the printing of them to the console
int errorHandle(int error, ...)
{
	va_list args;
	char *message;
	printf("ERROR: ");
	switch(error)
	{
		case E_SDL_INIT:
			message = "SDL failed to initilaise\nSDL Error: %s\n";
			break;
		case E_SDL_WINDOW_CREATE:
			message = "Failed to create window\nSDL Error: %s\n";
			break;
		case E_SDL_LOAD_IMAGE:
			message = "Failed to load image %s\nSDL Error: %s\n";
			break;
		case E_INIT:
			message = "Failed to initialise programme\n";
			break;
		case E_LOAD_MEDIA:
			message = "Failed to load media\n";
			break;
		case E_FILE_NOT_FOUND:
			message = "Failed to find the specified file %s\n";
			break;
		case E_BUFFER_NULL:
			message = "Buffer returned NULL during memory allocation\n";
			break;
		case E_FILE_READ:
			message = "Failed to read file %s\n";
			break;
		case E_SDL_OPTOMISE_BMP:
			message = "SDL failed to optimise the specified Bitmap file %s\nSDL Error: %s\n";
			break;
		case E_SDL_RENDERER_CREATE:
			message = "Failed to create renderer\nSDL Error: %s\n";
			break;
		case E_SDL_CREATE_TEXTURE_IMAGE:
			message = "Failed to create texture from image %s\nSDL Error: %s\n";
			break;
		case E_UNSUPPORTED_ENTITY_THING_TYPE:
			message = "Tried to access an entity that references a variable with an unsupported data form %d\n";
			break;
		case E_ENTITY_NULL:
			message = "Entity to be accessed returns NULL\n";
			break;
		default:
			message = "Unknown error\n";
			break;
	}
	va_start(args, error);
	vprintf(message, args);
	va_end(args);
	quit = true;
	return 0;
}

// Reads file
char *fileRead(const char *fileName)
{
    FILE *file;
    char *buffer;
    long fileSize;
    size_t result;

    file = fopen(fileName, "r");
    if (file == NULL)
    {
        errorHandle(E_FILE_NOT_FOUND);
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc(sizeof(char) * fileSize);
    if (buffer == NULL)
    {
        errorHandle(E_BUFFER_NULL);
    }

    result = fread(buffer, 1, fileSize, file);
    if (result != fileSize)
    {
        errorHandle(E_FILE_READ);
    }

    return buffer;
}

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

// Creates entitycount here
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

// Loads surface, but can't be used with textures/rendering stuff
SDL_Surface* loadSurface(char *path)
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
mTexture loadTexture(char* path, int destTexX, int destTexY, int destTexW, int destTexH)
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
int loadMedia(entity **list, int *counter)
{
	bgTextureM = loadTexture("img/bg.bmp", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	bgTexture = createEntity(&bgTextureM, T_MTEXTURE, 0, 0, list, counter);
	return 0;
}

// Initialise SDL in programme
int init()
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
			// Get window surface
			//screenSurface = SDL_GetWindowSurface(window);
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
	if (init())
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

			// Set up camera
			int *cameraTransform = centreGraphic(SCREEN_WIDTH, SCREEN_HEIGHT);
			SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			free(cameraTransform);

			// Variables required to be declared before main loop due to memory allocation
			int *rectTransform = centreGraphic((SCREEN_WIDTH / 6), (SCREEN_HEIGHT / 6));
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
				SDL_Rect temp = *(SDL_Rect*)redRect.thing;
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
