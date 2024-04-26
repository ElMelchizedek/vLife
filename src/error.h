#ifndef ERROR_H
#define ERROR_H

// Error macros
#define E_SDL_INIT 0
#define E_SDL_WINDOW_CREATE 1
#define E_INIT 2
#define E_MEM 3
#define E_SDL_RENDERER_CREATE 4
#define E_GRID_FLOAT 5
#define E_REALLOC 6

int errorHandle(int error, ...);

#endif // ERROR_H