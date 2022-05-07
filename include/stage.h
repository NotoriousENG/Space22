#pragma once

#include "structs.h"
#include "sound.h"
#include "text.h"

extern void blit(SDL_Texture* texture, int x, int y);
extern void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
extern void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy);
extern int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void loadMusic(char* filename);
extern SDL_Texture* loadTexture(char* filename);
extern void playMusic(int loop);
extern void playSound(int id, int channel);
extern void drawText(int x, int y, int r, int g, int b, char* format, ...);

extern App app;
extern Stage stage;