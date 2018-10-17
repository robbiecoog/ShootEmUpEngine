#include "EngineManager.h"


EngineManager::EngineManager()
{
	//initalize SDL
	Init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, false);

	//Init UI Manager
	_UIManager = new UIManager(window, renderer);

	//Sound Manager
	_SoundManager = SoundManager::getInstance();
}

void EngineManager::Init(const char* name, int xPosition, int yPosition, int width, int height, bool isFullscreen)
{
	backColor.r = 200; backColor.g = 200; backColor.b = 255; backColor.a = 255;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0)
	{
		int screenFlag = 0;
		if (isFullscreen) screenFlag = SDL_WINDOW_FULLSCREEN;

		window = SDL_CreateWindow(name, xPosition, yPosition, width, height, screenFlag);

		renderer = SDL_CreateRenderer(window, -1, 0);

		SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);

		gameIsRunning = true;
	}
	else
	{
		gameIsRunning = false;
	}
}

EngineManager::~EngineManager()
{
	_SoundManager->~SoundManager();
	_UIManager->~UIManager();
	delete _UIManager;
}

void EngineManager::HandleEvents()
{

}

void EngineManager::Update()
{
	//as the other update functions will use events, to prevent recreation of these and interference, this is passed through
	SDL_Event* e = new SDL_Event;
	SDL_PollEvent(e);

	_UIManager->Update(e);
	_SoundManager->Update(e);

	e = nullptr;

}

void EngineManager::Draw()
{
	SDL_RenderClear(renderer);

	_UIManager->Draw();

	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

void EngineManager::Clean()
{

}