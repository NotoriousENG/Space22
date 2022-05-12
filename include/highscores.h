#pragma once

#include "draw.h"
#include "stage.h"
#include "background.h"
#include "title.h"

void initHighscoreTable(void);
void initHighscores(void);
static void logic(void);
static void doNameInput(void);
static void draw(void);
static void drawNameInput(void);
static void drawHighscores(void);
void addHighscore(int score);
static int highscoreComparator(const void *a, const void *b);
void write_highscores();
void read_highscores();

extern App app;
extern Highscores highscores;