#include "highscores.h"

static void logic(void);
static void draw(void);
static int highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);

static Highscore *newHighscore;
static int cursorBlink;

void initHighscoreTable(void)
{
	int i;
	
	memset(&highscores, 0, sizeof(Highscores));
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		highscores.highscore[i].score = NUM_HIGHSCORES - i;
		STRNCPY(highscores.highscore[i].name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
	}
	
	newHighscore = NULL;
	
	cursorBlink = 0;
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
	
	if (newHighscore != NULL)
	{
		doNameInput();
	}
	else
	{
		if (app.keyboard[SDL_SCANCODE_LCTRL])
		{
			initStage();
		}
	}
	
	if (++cursorBlink >= FPS)
	{
		cursorBlink = 0;
	}
}

static void doNameInput(void)
{
	int i, n;
	char c;
	
	n = strlen(newHighscore->name);
	
	for (i = 0 ; i < strlen(app.inputText) ; i++)
	{
		c = toupper(app.inputText[i]);
		
		// alpha characters only
		if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z')
		{
			newHighscore->name[n++] = c;
		}
	}
	
	if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE])
	{
		newHighscore->name[--n] = '\0';
		
		app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
	}
	
	// entered name
	if (app.keyboard[SDL_SCANCODE_RETURN])
	{
		if (strlen(newHighscore->name) == 0)
		{
			STRNCPY(newHighscore->name, "ANONYMOUS", MAX_SCORE_NAME_LENGTH);
		}

		SDL_StopTextInput();
		
		newHighscore = NULL;
	}
}

static void draw(void)
{
	drawBackground();
	
	drawStarfield();
	
	if (newHighscore != NULL)
	{
		drawNameInput();
	}
	else
	{
		drawHighscores();
	}
}

static void drawNameInput(void)
{
	SDL_Rect r;
	
	drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, "CONGRATULATIONS, YOU'VE GAINED A HIGHSCORE!");
	
	drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255, TEXT_CENTER, "ENTER YOUR NAME BELOW:");
	
	drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, TEXT_CENTER, newHighscore->name);
	
	// draw cursor
	if (cursorBlink < FPS / 2)
	{
		r.x = ((SCREEN_WIDTH / 2) + (strlen(newHighscore->name) * GLYPH_WIDTH) / 2) + 5;
		r.y = 250;
		r.w = GLYPH_WIDTH;
		r.h = GLYPH_HEIGHT;
		
		SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(app.renderer, &r);
	}
	
	drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, TEXT_CENTER, "PRESS RETURN WHEN FINISHED");
}

// draw each highscore with a gap of 50 between each, start prompt at bottom
static void drawHighscores(void)
{
	int i, y, r, g, b;
	
	y = 150;
	
	drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, "HIGHSCORES");
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		r = 255;
		g = 255;
		b = 255;
		
		if (highscores.highscore[i].recent)
		{
			b = 0;
		}
		
		drawText(SCREEN_WIDTH / 2, y, r, g, b, TEXT_CENTER, "#%d. %-15s ...... %03d", (i + 1), highscores.highscore[i].name, highscores.highscore[i].score);
		
		y += 50;
	}
	
	drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, "PRESS FIRE TO PLAY!");
}


// add old and new highscores to temp array, sort and copy the first NUM_HIGHSCORES back to highscore array, 
// store the new highscore to enter name
void addHighscore(int score)
{
	Highscore newHighscores[NUM_HIGHSCORES + 1];
	int i;
	
	memset(newHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		newHighscores[i] = highscores.highscore[i];
		newHighscores[i].recent = 0;
	}
	
	newHighscores[NUM_HIGHSCORES].score = score;
	newHighscores[NUM_HIGHSCORES].recent = 1;
	
	qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);
	
	newHighscore = NULL;
	
	for (i = 0 ; i < NUM_HIGHSCORES ; i++)
	{
		highscores.highscore[i] = newHighscores[i];
		
		if (highscores.highscore[i].recent)
		{
			SDL_StartTextInput();
			newHighscore = &highscores.highscore[i];
		}
	}
}

// highscore comparison for qsort
static int highscoreComparator(const void *a, const void *b)
{
	Highscore *h1 = ((Highscore*)a);
	Highscore *h2 = ((Highscore*)b);

	return h2->score - h1->score;
}

void write_highscores()
{
    FILE *fptr;

    if ((fptr = fopen("highscores.bin", "wb")) == NULL)
    {
        printf("Error! opening file");

        // returns if the file pointer returns NULL.
        return;
    }

    fwrite(&highscores, sizeof(Highscores), 1, fptr);
    fclose(fptr);
}

void read_highscores()
{
    FILE *fptr;

    if ((fptr = fopen("highscores.bin", "rb")) == NULL)
    {
        printf("Error! opening file");

        // sets default values if the file pointer returns NULL.
        return initHighscoreTable();
    }

    fread(&highscores, sizeof(Highscores), 1, fptr);
    fclose(fptr);

	newHighscore = NULL;
	cursorBlink = 0;
}