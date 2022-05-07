#pragma once
#include "structs.h"
#include "stage.h"
#include "text.h"
#include "highscores.h"
#include "background.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

extern void initBackground(void);
extern void initFonts(void);
extern void initHighscoreTable(void);
extern void initSounds(void);
extern void initStarfield(void);
extern void loadMusic(char *filename);
extern void playMusic(int loop);

extern App app;