#pragma once

#include "structs.h"

void doKeyUp(SDL_KeyboardEvent* event);
void doKeyDown(SDL_KeyboardEvent* event);
void doInput(void);

extern App app;