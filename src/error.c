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
		case E_INIT:
			message = "Failed to initialise programme\n";
			break;
		case E_MEM:
			message = "Failed to allocate memory for %s\n";
			break;
		case E_SDL_RENDERER_CREATE:
			message = "Failed to create renderer\nSDL Error: %s\n";
			break;
		case E_GRID_FLOAT:
			message = "Calculated cell grid from level width %d, or height %d, is non-integer and will break cell initialisation\n";
			break;
		case E_REALLOC:
			message = "Faied to reallocate memory for variable %s\n";
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