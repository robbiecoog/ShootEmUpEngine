#include "Animator.h"



Animator::Animator(SDL_Renderer* gameRenderer, SDL_Window* gameWindow)
{
	renderer = gameRenderer;
	window = gameWindow;

	spriteImages = IMG_Load(_FileDialog->GetFile().c_str());
	spriteSheetTexture = SDL_CreateTextureFromSurface(gameRenderer, spriteImages);

	std::cout << "Error?: " << SDL_GetError();

	animationTicks = SDL_GetTicks(); //gives number of milliseconds  that passed since program started
	sprite = (animationTicks / 100) % 4;

	srcRect = {  (int)(sprite * 160), 0, 160, 479 };
	destRect = { 10,10,32,64 };
}


Animator::~Animator()
{
	SDL_DestroyTexture(spriteSheetTexture);
	SDL_FreeSurface(spriteImages);
}

void Animator::Update(SDL_Event* e)
{
	animationTicks = SDL_GetTicks(); //gives number of milliseconds  that passed since program started
	sprite = (animationTicks / 100) % 4;

	srcRect = { (int)(sprite * 160), 0, 160, 479 };

	if (sprite > 4)
	{
		sprite = 0;
	}
}

void Animator::Draw()
{
	SDL_RenderCopy(renderer, spriteSheetTexture, &srcRect, &destRect);
}