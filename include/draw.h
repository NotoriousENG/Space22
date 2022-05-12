#pragma once

#include "structs.h"

#include <SDL.h>
#include "SDL2/SDL_image.h"

void prepareScene(void);
void presentScene(void);
static SDL_Texture *getTexture(char *name);
static void addTextureToCache(char *name, SDL_Texture *sdlTexture);
SDL_Texture *loadTexture(char *filename);
void blit(SDL_Texture* texture, int x, int y);\
void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);

extern App app;