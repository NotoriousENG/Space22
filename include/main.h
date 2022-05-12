#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <SDL.h>

#include "defs.h"
#include "structs.h"
#include "sound.h"
#include "text.h"
#include "highscores.h"
#include "init.h"
#include "input.h"

int main(int argc, char *argv[]);
void main_loop();
static void capFrameRate(long *then, float *remainder);

App app;
Stage stage;
Highscores highscores;