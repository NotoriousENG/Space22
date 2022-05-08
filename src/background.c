#include "background.h"

int backgroundX;
Star stars[MAX_STARS];
SDL_Texture *background;

void initBackground(void)
{
	background = loadTexture("assets/sprites/bg.png");

	backgroundX = 0;
}

// loop background across the screen
void doBackground(void)
{
	if (--backgroundX < -SCREEN_WIDTH)
	{
		backgroundX = 0;
	}
}

// loop stars across the screen
void doStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x -= stars[i].speed;

		if (stars[i].x < 0)
		{
			stars[i].x = SCREEN_WIDTH + stars[i].x;
		}
	}
}

void initStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}

// draw backround looped to cover the screen
void drawBackground(void)
{
	SDL_Rect dest;
	int x;

	for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = SCREEN_HEIGHT;

		SDL_RenderCopy(app.renderer, background, NULL, &dest);
	}
}

// draw each stars, faster stars are brighter
void drawStarfield(void)
{
	int i, c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}