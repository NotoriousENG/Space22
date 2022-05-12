#pragma once

#include <SDL_mixer.h>

void initSounds(void);
static void loadSounds(void);
void loadMusic(char* filename);
void playMusic(int loop);
void playSound(int id, int channel);