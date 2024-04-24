#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

extern bool quit;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

void saveAddress(void*** addressList, int* addressCount, void* selectThing);

#endif // MAIN_H