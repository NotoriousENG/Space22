#ifdef EMSCRIPTEN
	#include <emscripten.h>
	#include <emscripten/html5.h>
#endif

#include "main.h"

long _then;
float _remainder;

int main(int argc, char *argv[])
{
	

	memset(&app, 0, sizeof(App));
	app.textureTail = &app.textureHead;

	initSDL();

	SDL_StopTextInput();

	atexit(cleanup);

	initGame();

	initTitle();

	_then = SDL_GetTicks();

	_remainder = 0;

#ifdef EMSCRIPTEN
emscripten_set_main_loop(main_loop, 0, 1);
#else
	while (1)
	{
		main_loop();
	}
#endif

	return 0;
}

void main_loop()
{
	prepareScene();

	doInput();

	app.delegate.logic();

	app.delegate.draw();

	presentScene();

	capFrameRate(&_then, &_remainder);
}

static void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;

	// 1000 / 60 FPS - 16.66667
	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1)
	{
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}