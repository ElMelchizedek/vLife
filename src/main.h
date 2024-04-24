#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

extern bool quit;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

int *saveAddress(int* addressList, int* addressCount, uintptr_t selectThing);

#endif // MAIN_H