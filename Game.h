#pragma once
#include "GameBase.h"
#include "timer.h"
using namespace timer;
#include "Player.h"
#include "Obstacle.h"
#include "random.h"
// Is this working?
enum GameState
{
	GAMESTATE_ACTIVE,
	GAMESTATE_SPLASH,
	GAMESTATE_RESET,
	GAMESTATE_0
};

using namespace random;

const int NUM_OBSTACLES = 10;

class Game :
	public GameBase
{
	Obstacle* obstacle[NUM_OBSTACLES];
	Timer *m_timer, *m_gameTime;
	SDL_Rect m_cam;
	Player *p;
	GameState currState;
	AWSprite splashSprite;

	int m_currTime;
	int m_winTime;
public:
	Game();
	~Game();
	void setup();
	void logic();
	void draw();
	void onKeyPressed();

	void updateCamera();
	void playerPos();
	void moveObstacles();
	void checkForWin();
	void performCollisions();
	void updateScore();

	void reset();

	void displayScore();
};

