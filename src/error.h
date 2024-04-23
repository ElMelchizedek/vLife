#ifndef ERROR_H
#define ERROR_H

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
#define E_CELL_NULL 13
#define E_ENTITY_MEM 14
#define E_CELL_DATA_MEM 15
#define E_GRID_FLOAT 16
#define E_CELL_THING_MEM 17
#define E_CELL_GRAPHICS_MEM 18

int errorHandle(int error, ...);

#endif // ERROR_H