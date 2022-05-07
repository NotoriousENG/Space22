#include "sound.h"
#include "defs.h"

#include <SDL2/SDL_mixer.h>

static void loadSounds(void);

static Mix_Chunk* sounds[SND_MAX];
static Mix_Music* music;

void initSounds(void)
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

	music = NULL;

	loadSounds();
}

// loads sounds into memory
static void loadSounds(void)
{
	sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("assets/sound/334227__jradcoolness__laser.ogg");
	sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("assets/sound/196914__dpoggioli__laser-gun.ogg");
	sounds[SND_PLAYER_DIE] = Mix_LoadWAV("assets/sound/245372__quaker540__hq-explosion.ogg");
	sounds[SND_ALIEN_DIE] = Mix_LoadWAV("assets/sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");
	sounds[SND_POINTS] = Mix_LoadWAV("assets/sound/342749__rhodesmas__notification-01.ogg");
}

// loads music into memory
void loadMusic(char* filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(filename);
	if (music == NULL)
	{
		printf("Music Error [is SDL_Mixer linked properly?] %s\n", Mix_GetError());
	}
	
}

// pass -1 to loop forever, else plays once
void playMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}