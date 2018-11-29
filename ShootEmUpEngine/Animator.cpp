#include "Animator.h"



Animator::Animator(SDL_Renderer* gameRenderer, SDL_Window* gameWindow)
{
	renderer = gameRenderer;
	window = gameWindow;

	spriteImages = IMG_Load(_FileDialog->GetFile().c_str());
	spriteSheetTexture = SDL_CreateTextureFromSurface(gameRenderer, spriteImages);

	std::cout << "Error?: " << SDL_GetError();
	frames = 1;
}


Animator::~Animator()
{
	SDL_DestroyTexture(spriteSheetTexture);
	SDL_FreeSurface(spriteImages);
}

void Animator::Update(SDL_Event* e)
{
	animationTicks = SDL_GetTicks(); //gives number of milliseconds  that passed since program started

	if (frames > 0 && spriteImages)
	{
		sprite = (animationTicks / 100) % frames;
		int frameWidth = (spriteImages->clip_rect.w / frames);

		srcRect = { (int)(sprite * frameWidth), 0, frameWidth, 479 };
		destRect = spriteImages->clip_rect;
		destRect.w = frameWidth;

		if (sprite > frames)
		{
			sprite = 0;
		}
	}
}

void Animator::SetFrames(int inputFrames)
{
	frames = inputFrames;
}

void Animator::Draw()
{
	SDL_RenderCopy(renderer, spriteSheetTexture, &srcRect, &destRect);
}