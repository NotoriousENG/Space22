#include "stage.h"
#include "defs.h"
#include "util.h"
#include "draw.h"

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doFighters(void);
static void doBullets(void);
static void drawFighters(void);
static void drawBullets(void);
static void spawnEnemies(void);
static int bulletHitFighter(Entity* b);
static void doEnemies(void);
static void fireAlienBullet(Entity* e);
static void clipPlayer(void);
static void resetStage(void);
static void drawBackground(void);
static void initStarfield(void);
static void drawStarfield(void);
static void doBackground(void);
static void doStarfield(void);
static void drawExplosions(void);
static void doExplosions(void);
static void addExplosions(int x, int y, int num);
static void addDebris(Entity* e);
static void doDebris(void);
static void drawDebris(void);
static void drawHud(void);
static void doPointsPods(void);
static void drawPointsPods(void);
static void addPointsPod(int x, int y);

static Entity* player;
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* alienBulletTexture;
static SDL_Texture* playerTexture;
static SDL_Texture* background;
static SDL_Texture* explosionTexture;
static SDL_Texture* pointsTexture;
static int enemySpawnTimer;
static int stageResetTimer;
static int backgroundX;
static Star stars[MAX_STARS];
static int highscore;

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;

	initPlayer();

	bulletTexture = loadTexture("assets/sprites/playerBullet.png");
	enemyTexture = loadTexture("assets/sprites/enemy.png");
	alienBulletTexture = loadTexture("assets/sprites/alienBullet.png");
	playerTexture = loadTexture("assets/sprites/player.png");
	background = loadTexture("assets/sprites/background.png");
	explosionTexture = loadTexture("assets/sprites/explosion.png");
	pointsTexture = loadTexture("assets/sprites/points.png");

	loadMusic("assets/music/Mercury.ogg");

	playMusic(1);

	resetStage();
}

static void resetStage(void)
{
	Entity* e;
	Explosion* ex;
	Debris* d;

	// free all fighters
	while (stage.fighterHead.next)
	{
		e = stage.fighterHead.next;
		stage.fighterHead.next = e->next;
		free(e);
	}

	// free all bullets
	while (stage.bulletHead.next)
	{
		e = stage.bulletHead.next;
		stage.bulletHead.next = e->next;
		free(e);
	}

	// free all explosions
	while (stage.explosionHead.next)
	{
		ex = stage.explosionHead.next;
		stage.explosionHead.next = ex->next;
		free(ex);
	}

	// free all debris
	while (stage.debrisHead.next)
	{
		d = stage.debrisHead.next;
		stage.debrisHead.next = d->next;
		free(d);
	}

	// free all points pods
	while (stage.pointsHead.next)
	{
		e = stage.pointsHead.next;
		stage.pointsHead.next = e->next;
		free(e);
	}

	// clears stage and reset linked list tails
	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.pointsTail = &stage.pointsHead;

	initPlayer();

	initStarfield();

	enemySpawnTimer = 0;

	stageResetTimer = FPS * 2;
}

static void initPlayer()
{
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));
	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->side = SIDE_PLAYER;

	player->health = 1;
	player->x = 100;
	player->y = 100;
	player->texture = playerTexture;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

static void initStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}

static void logic(void)
{
	doBackground();

	doStarfield();

	doPlayer();

	doEnemies();

	doFighters();

	doBullets();

	doExplosions();

	doDebris();

	doPointsPods();

	spawnEnemies();

	clipPlayer();

	if (player == NULL && --stageResetTimer <= 0)
	{
		resetStage();
	}
}

// loop background across the screen
static void doBackground(void)
{
	if (--backgroundX < -SCREEN_WIDTH)
	{
		backgroundX = 0;
	}
}

// loop stars across the screen
static void doStarfield(void)
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

static void doPlayer(void)
{
	if (player != NULL)
	{
		player->dx = player->dy = 0;

		if (player->reload > 0)
		{
			player->reload--;
		}

		if (app.keyboard[SDL_SCANCODE_UP])
		{
			player->dy = -PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_DOWN])
		{
			player->dy = PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_LEFT])
		{
			player->dx = -PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_RIGHT])
		{
			player->dx = PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
		{
			fireBullet();
			playSound(SND_PLAYER_FIRE, CH_PLAYER);
		}
	}
}

// fire bullets at player if possible
static void doEnemies(void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if (e != player && player != NULL && --e->reload <= 0)
		{
			fireAlienBullet(e);
			playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
		}
	}
}

static void fireBullet(void)
{
	Entity* bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->side = SIDE_PLAYER;
	bullet->x = player->x;
	bullet->y = player->y;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

	player->reload = 8;
}

// fires a bullet in direction of player
static void fireAlienBullet(Entity* e)
{
	Entity* bullet;

	bullet = malloc(sizeof(Entity));
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = e->x;
	bullet->y = e->y;
	bullet->health = 1;
	bullet->texture = alienBulletTexture;
	bullet->side = e->side;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->x += (e->w / 2) - (bullet->w / 2);
	bullet->y += (e->h / 2) - (bullet->h / 2);

	calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

	bullet->dx *= ALIEN_BULLET_SPEED;
	bullet->dy *= ALIEN_BULLET_SPEED;

	bullet->side = SIDE_ALIEN;

	e->reload = (rand() % FPS * 2);
}


// move each fighter, destroy enemies when offscreen
static void doFighters(void)
{
	Entity* e, * prev;

	prev = &stage.fighterHead;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if (e != player && e->x < -e->w)
		{
			e->health = 0;
		}

		if (e->health == 0)
		{
			if (e == player)
			{
				player = NULL; // NULL player so that we don't have a dangling pointer when we free
			}

			if (e == stage.fighterTail)
			{
				stage.fighterTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

/// <summary>
/// for bullets, move position, remove from linked list when reaching screen edge or hit by an oppossing bullet and free memory
/// </summary>
/// <param name=""></param>
static void doBullets(void)
{
	Entity* b, * prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;

		// x or y coordinate (plus w and h) is outside of the screen or health is 0
		if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
		{
			if (b == stage.bulletTail)
			{
				stage.bulletTail = prev;
			}

			prev->next = b->next;
			free(b);
			b = prev;
		}

		prev = b;
	}
}

// check collision for a fighter and apply damage
static int bulletHitFighter(Entity* b)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
		{
			b->health = 0;
			e->health = 0;

			addExplosions(e->x, e->y, 32);

			addDebris(e);

			if (e == player)
			{
				playSound(SND_PLAYER_DIE, CH_PLAYER);
			}
			else
			{
				playSound(SND_ALIEN_DIE, CH_ANY);

				addPointsPod(e->x + e->w / 2, e->y + e->h / 2);
			}

			return 1;
		}
	}

	return 0;
}

// spawn enemies by timer cycle, at a random height alongside the right edge of the screen
static void spawnEnemies(void)
{
	Entity* enemy;

	if (--enemySpawnTimer <= 0)
	{
		enemy = malloc(sizeof(Entity));
		memset(enemy, 0, sizeof(Entity));
		stage.fighterTail->next = enemy;
		stage.fighterTail = enemy;

		enemy->side = SIDE_ALIEN;
		enemy->x = SCREEN_WIDTH;
		enemy->y = rand() % SCREEN_HEIGHT;
		enemy->health = 1;
		enemy->texture = enemyTexture;
		enemy->reload = FPS * (1 + (rand() % 3)); // wait for a few seconds after creation to fire
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

		enemy->dx = -(2 + (rand() % 4));

		enemySpawnTimer = 30 + (rand() % FPS);
	}
}

static void clipPlayer(void)
{
	if (player != NULL)
	{
		if (player->x < 0)
		{
			player->x = 0;
		}

		if (player->y < 0)
		{
			player->y = 0;
		}

		if (player->x > SCREEN_WIDTH - player->w)
		{
			player->x = SCREEN_WIDTH - player->w;
		}

		if (player->y > SCREEN_HEIGHT - player->h)
		{
			player->y = SCREEN_HEIGHT - player->h;
		}
	}
}

// move explosions, decrement alpha, delete when alpha <= 0
static void doExplosions(void)
{
	Explosion* e, * prev;

	prev = &stage.explosionHead;

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if (--e->a <= 0)
		{
			if (e == stage.explosionTail)
			{
				stage.explosionTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

// have debris fall off screen over time and destroy
static void doDebris(void)
{
	Debris* d, * prev;

	prev = &stage.debrisHead;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		d->x += d->dx;
		d->y += d->dy;

		d->dy += 0.5;

		if (--d->life <= 0)
		{
			if (d == stage.debrisTail)
			{
				stage.debrisTail = prev;
			}

			prev->next = d->next;
			free(d);
			d = prev;
		}

		prev = d;
	}
}

// move points pods, handle collision with player to earn points, hanlde freeing memory
static void doPointsPods(void)
{
	Entity* e, * prev;

	prev = &stage.pointsHead;

	for (e = stage.pointsHead.next; e != NULL; e = e->next)
	{
		if (e->x < 0)
		{
			e->x = 0;
			e->dx = -e->dx;
		}

		if (e->x + e->w > SCREEN_WIDTH)
		{
			e->x = SCREEN_WIDTH - e->w;
			e->dx = -e->dx;
		}

		if (e->y < 0)
		{
			e->y = 0;
			e->dy = -e->dy;
		}

		if (e->y + e->h > SCREEN_HEIGHT)
		{
			e->y = SCREEN_HEIGHT - e->h;
			e->dy = -e->dy;
		}

		e->x += e->dx;
		e->y += e->dy;

		if (player != NULL && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))
		{
			e->health = 0;

			stage.score++;

			highscore = MAX(stage.score, highscore);

			playSound(SND_POINTS, CH_POINTS);
		}

		if (--e->health <= 0)
		{
			if (e == stage.pointsTail)
			{
				stage.pointsTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

// add a number of explosions with random colors red - orange - yellow - white with a random lifetime (alpha)
static void addExplosions(int x, int y, int num)
{
	Explosion* e;
	int i;

	for (i = 0; i < num; i++)
	{
		e = malloc(sizeof(Explosion));
		memset(e, 0, sizeof(Explosion));
		stage.explosionTail->next = e;
		stage.explosionTail = e;

		e->x = x + (rand() % 32) - (rand() % 32);
		e->y = y + (rand() % 32) - (rand() % 32);
		e->dx = (rand() % 10) - (rand() % 10);
		e->dy = (rand() % 10) - (rand() % 10);

		e->dx /= 10;
		e->dy /= 10;

		switch (rand() % 4)
		{
		case 0:
			e->r = 255;
			break;

		case 1:
			e->r = 255;
			e->g = 128;
			break;

		case 2:
			e->r = 255;
			e->g = 255;
			break;

		default:
			e->r = 255;
			e->g = 255;
			e->b = 255;
			break;
		}

		e->a = rand() % FPS * 3;
	}
}

// shatter entity into 4 pieces when destroyed
static void addDebris(Entity* e)
{
	Debris* d;
	int x, y, w, h;

	w = e->w / 2.0f;
	h = e->h / 2.0f;

	for (y = 0; y <= h; y += h)
	{
		for (x = 0; x <= w; x += w)
		{
			d = malloc(sizeof(Debris));
			memset(d, 0, sizeof(Debris));
			stage.debrisTail->next = d;
			stage.debrisTail = d;

			d->x = e->x + e->w / 2.0f;
			d->y = e->y + e->h / 2.0f;
			d->dx = (rand() % 5) - (rand() % 5);
			d->dy = -(5 + (rand() % 12));
			d->life = FPS * 2;
			d->texture = e->texture;

			d->rect.x = x;
			d->rect.y = y;
			d->rect.w = w;
			d->rect.h = h;
		}
	}
}

// add a points pod that will be live for 10 seconds
static void addPointsPod(int x, int y)
{
	Entity* e;

	e = malloc(sizeof(Entity));
	memset(e, 0, sizeof(Entity));
	stage.pointsTail->next = e;
	stage.pointsTail = e;

	e->x = x;
	e->y = y;
	e->dx = -(rand() % 5);
	e->dy = (rand() % 5) - (rand() % 5);
	e->health = FPS * 10;
	e->texture = pointsTexture;

	SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

	e->x -= e->w / 2;
	e->y -= e->h / 2;
}

static void draw(void)
{
	drawBackground();

	drawStarfield();

	drawPointsPods();

	drawFighters();

	drawBullets();

	drawDebris();

	drawExplosions();

	drawBullets();

	drawHud();
}

static void drawPointsPods(void)
{
	Entity* e;

	for (e = stage.pointsHead.next; e != NULL; e = e->next)
	{
		blit(e->texture, e->x, e->y);
	}
}

static void drawFighters(void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		blit(e->texture, e->x, e->y);
	}
}

static void drawBullets(void)
{
	Entity* b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		blit(b->texture, b->x, b->y);
	}
}

// draw backround looped to cover the screen
static void drawBackground(void)
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
static void drawStarfield(void)
{
	int i, c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}

// draw each debris item
static void drawDebris(void)
{
	Debris* d;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		blitRect(d->texture, &d->rect, d->x, d->y);
	}
}

// draw explosions with a glow effect
static void drawExplosions(void)
{
	Explosion* e;

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(explosionTexture, e->a);

		blit(explosionTexture, e->x, e->y);
	}

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawHud(void)
{
	drawText(10, 10, 255, 255, 255, "SCORE: %03d", stage.score);

	if (stage.score > 0 && stage.score == highscore)
	{
		drawText(1020, 10, 0, 255, 0, "HIGHSCORE: %03d", highscore);
	}
	else
	{
		drawText(1020, 10, 255, 255, 255, "HIGHSCORE: %03d", highscore);
	}
}