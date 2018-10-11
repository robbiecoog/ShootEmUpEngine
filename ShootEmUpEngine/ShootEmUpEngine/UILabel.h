#pragma once

#include "SDL.h"
#include "SDL_TTF.h"
#include <string>
#include <iostream>

class UILabel
{

public:
	UILabel(int inputX, int inputY, std::string inputText, SDL_Renderer* gameRenderer, int inputAlignment);
	~UILabel();

	void Update(SDL_Event* e);

	void Draw();

private:
	int x, y;
	std::string text;

	SDL_Renderer* renderer;

	SDL_Color textColor;

	TTF_Font* defaultFont;

	int alignment;
};