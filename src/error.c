#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "error.h"
#include "main.h"


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
		case E_CELL_NULL:
			message = "Cell to be accessed returns NULL\n";
			break;
		case E_ENTITY_MEM:
			message = "Failed to allocate memory for new entity\n";
			break;
		case E_CELL_DATA_MEM:
			message = "Failed to allocate memory for new cellData\n";
			break;
		case E_GRID_FLOAT:
			message = "Calculated cell grid from level width %d, or height %d, is non-integer and will break cell initialisation\n";
			break;
		case E_CELL_GRAPHICS_MEM:
			message = "Failed to allocate memory for new cellGraphics\n";
			break;
		case E_CELL_THING_MEM:
			message = "Failed to allocate memory for new cellThing\n";
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