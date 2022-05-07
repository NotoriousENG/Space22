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

extern void cleanup(void);
extern void doInput(void);
extern void initFonts(void);
extern void initSDL(void);
extern void initSounds(void);
extern void initStage(void);
extern void prepareScene(void);
extern void presentScene(void);

App app;
Stage stage;