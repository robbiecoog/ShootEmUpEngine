#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Animator
{
public:
	Animator(SDL_Renderer* gameRenderer, SDL_Window* window);
	~Animator();

	void Update(SDL_Event* e);

	void Draw();

private:
	SDL_Surface* spriteImages;
	SDL_Texture* spriteSheetTexture;

	SDL_Renderer* renderer;
	SDL_Window* window;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	Uint32 animationTicks;
	Uint32 sprite;

};

