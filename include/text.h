#pragma once
#include "draw.h"

#define GLYPH_HEIGHT    28
#define GLYPH_WIDTH     18

extern void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
extern SDL_Texture* loadTexture(char* filename);


void initFonts(void);
void drawText(int x, int y, int r, int g, int b, int align, char *format, ...);