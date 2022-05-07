#include "highscores.h"

static void logic(void);
static void draw(void);
static int highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);

void initHighscoreTable(void)
{
	int i;
	
	memset(&highscores, 0, sizeof(Highscores));
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		highscores.highscore[i].score = NUM_HIGHSCORES - i;
	}
}

// initialize high score "scene", switch logic and draw delegates
void initHighscores(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
    // clear keyboard entries to prevent missing this "scene"
	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);
}

// draw bg and starfield, when fire is pressed start game scene
static void logic(void)
{
	doBackground();
	
	doStarfield();
	
	if (app.keyboard[SDL_SCANCODE_LCTRL])
	{
		initStage();
	}
}

static void draw(void)
{
	drawBackground();
	
	drawStarfield();
	
	drawHighscores();
}

// draw each highscore with a gap of 50 between each, start prompt at bottom
static void drawHighscores(void)
{
	int i, y;
	
	y = 150;
	
	drawText(425, 70, 255, 255, 255, "HIGHSCORES");
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		if (highscores.highscore[i].recent)
		{
			drawText(425, y, 255, 255, 0, "#%d ............. %03d", (i + 1), highscores.highscore[i].score);
		}
		else
		{
			drawText(425, y, 255, 255, 255, "#%d ............. %03d", (i + 1), highscores.highscore[i].score);
		}
		
		y += 50;
	}
	
	drawText(425, 600, 255, 255, 255, "PRESS FIRE TO PLAY!");
}

// add old and new highscores to temp array, sort and copy the first NUM_HIGHSCORES back to highscore array
void addHighscore(int score)
{
	Highscore newHighscores[NUM_HIGHSCORES + 1];
	int i;
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		newHighscores[i] = highscores.highscore[i];
		newHighscores[i].recent = 0;
	}
	
	newHighscores[NUM_HIGHSCORES].score = score;
	newHighscores[NUM_HIGHSCORES].recent = 1;
	
	qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		highscores.highscore[i] = newHighscores[i];
	}
}

// highscore comparison for qsort
static int highscoreComparator(const void *a, const void *b)
{
	Highscore *h1 = ((Highscore*)a);
	Highscore *h2 = ((Highscore*)b);

	return h2->score - h1->score;
}