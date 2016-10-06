#include "Game.h"


Game::Game(){
	m_timer = NULL;
}

void Game::setup(){
	setBackground("images/splash1.png");

	m_timer = new Timer();

	currState = GAMESTATE_SPLASH;

	m_cam.x = m_cam.y = 0;
	m_cam.w = width;
	m_cam.h = height;

	p = new Player("images/fish.bmp");

	int ctr;
	int gap;

	AWSprite* tmp = new AWSprite("images/upperpipe.bmp", 1, 1);

	for (int i = 0; i < NUM_OBSTACLES/2; i++)
	{
		ctr = rnd.number(200, 300);
		gap = rnd.number(20, 60);
		obstacle[i] = new Obstacle("images/lowerpipe.bmp", 640 + i*200, ctr + gap);
		obstacle[i + 5] = new Obstacle("images/upperpipe.bmp", 640 + i * 200, ctr - gap - tmp->get_height());
	}

	m_gameTime = new Timer;
	m_currTime = 0;
	m_winTime = 100;

	SAFE_DELETE_PTR(tmp);
}

void Game::logic(void){
	
	if (currState == GAMESTATE_ACTIVE)
	{
		updateCamera();
		playerPos();
		moveObstacles();
		updateScore();
		performCollisions();
	}
	else if (currState == GAMESTATE_RESET)
	{
		reset();
	}
}

void Game::onKeyPressed(){
	if (keyDown == SDLK_SPACE && currState == GAMESTATE_ACTIVE)
	{
		p->jump();
	}
	else if (keyDown == SDLK_SPACE && currState == GAMESTATE_SPLASH)
	{
		setBackground("images/SeaBackground.png");
		currState = GAMESTATE_ACTIVE;
	}
		
}

void Game::updateCamera()
{
	m_cam.x = m_timer->getElapsedTime() * 50;
	if (m_cam.x + m_cam.w > bg->w){
		m_cam.x = 0;
		m_timer->resetTimer();
		for (int i = 0; i < NUM_OBSTACLES; i++)
		{
			obstacle[i]->reset();
		}
	}
}

void Game::playerPos()
{
	if (p->get_y() > 400){
		

		p->set_auto_accelerate(0);
		p->set_auto_move(0);
		p->set_world_position_y(400);
		currState = GAMESTATE_RESET;
	}
}

void Game::updateScore()
{
	int t = m_gameTime->getElapsedTime();
	if (t > m_currTime)
	{
		p->addToScore(5);
		m_currTime = t;
	}
}

void Game::performCollisions()
{
	for (int i = 0; i < NUM_OBSTACLES; i++)
	{
		if (p->pp_collision(obstacle[i]))
		{
			currState = GAMESTATE_RESET;
		}
	}

	
}

void Game::moveObstacles()
{
	for (int i = 0; i < NUM_OBSTACLES; i++)
	{
		if (m_cam.x + m_cam.w > obstacle[i]->getX() && !obstacle[i]->isMoving())
		{
			obstacle[i]->move();
		}
	}
}

void Game::displayScore()
{
	print(p->getScore(), 10, 30, WHITE);
	print(m_currTime, 10, 50, WHITE);
}

void Game::draw(void){
	

	SDL_RenderCopy(renderer, bgTexture, &m_cam, NULL);
	p->update_everything();
	for (int i = 0; i < NUM_OBSTACLES; i++)
	{
		obstacle[i]->update_everything();
	}
	displayScore();
}

void Game::reset()
{
	// Change Background
	setBackground("images/splash2.png");
	// Delete old obstacles
	for (int i = 0; i < NUM_OBSTACLES; i++)
	{
		SAFE_DELETE_PTR(obstacle[i]);
	}

	// Create new obstacles
	AWSprite* tmp = new AWSprite("images/upperpipe.bmp", 1, 1);
	int ctr;
	int height = tmp->get_height();
	int gap;

	for (int i = 0; i < NUM_OBSTACLES / 2; i++)
	{
		ctr = rnd.number(200, 300);
		gap = rnd.number(20, 60);
		obstacle[i] = new Obstacle("images/lowerpipe.bmp", 640 + i * 200, ctr + gap);
		obstacle[i + 5] = new Obstacle("images/upperpipe.bmp", 640 + i * 200, ctr - gap - height);

		SAFE_DELETE_PTR(tmp);
	}

	// Reset the timer
	SAFE_DELETE_PTR(m_timer);
	SAFE_DELETE_PTR(m_gameTime);
	m_timer = new Timer();
	m_gameTime = new Timer();
	m_currTime = 0;

	// Reset the Player
	SAFE_DELETE_PTR(p);
	p = new Player("images/fish.bmp");

	// Reset the camera
	m_cam.x = m_cam.y = 0;
	m_cam.w = width;
	m_cam.h = height;

	// Set the current State
	currState = GAMESTATE_SPLASH;

}

Game::~Game(){
	SAFE_DELETE_PTR(m_timer);
}
