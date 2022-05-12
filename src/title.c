#include "title.h"

static SDL_Texture *shootyTexture;
static SDL_Texture *shootTexture;
static int reveal = 0;
static int timeout;

void initTitle(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
	
	shootyTexture = loadTexture("assets/sprites/shooty.png");
	shootTexture = loadTexture("assets/sprites/shoot.png");
	
	timeout = FPS * 5;
}

static void logic(void)
{
	doBackground();
	
	doStarfield();
	
	if (reveal < SCREEN_HEIGHT)
	{
		reveal++;
	}
	
	if (--timeout <= 0)
	{
		initHighscores();
	}
	
	if (app.keyboard[SDL_SCANCODE_LCTRL])
	{
		initStage();
	}
}

static void draw(void)
{
	drawBackground();
	
	drawStarfield();
	
	drawLogo();
	
	if (timeout % 40 < 20)
	{
		drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY!");
	}
}

static void drawLogo(void)
{
	SDL_Rect r;
	
	r.x = 0;
	r.y = 0;
	
	SDL_QueryTexture(shootyTexture, NULL, NULL, &r.w, &r.h);
	
	r.h = MIN(reveal, r.h);
	
	blitRect(shootyTexture, &r, (SCREEN_WIDTH / 2) - (r.w / 2), 100);
	
	SDL_QueryTexture(shootTexture, NULL, NULL, &r.w, &r.h);
	
	r.h = MIN(reveal, r.h);
	
	blitRect(shootTexture, &r, (SCREEN_WIDTH / 2) - (r.w / 2), 350);
}