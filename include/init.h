#pragma once
#include "structs.h"
#include "stage.h"
#include "text.h"
#include "highscores.h"
#include "background.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

void initSDL(void);
void initGame(void);
void cleanup(void);

extern App app;