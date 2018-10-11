#pragma once

#include "SDL.h"
#include "SDL_TTF.h"
#include <string>

class UITextBox
{
	//This is an element of the UI. It is a field for entering data
public:
	UITextBox(int xPos, int yPos, int boxWidth, SDL_Renderer* gameRenderer);
	UITextBox(int xPos, int yPos, int boxWidth, std::string inputText, SDL_Renderer* gameRenderer);
	~UITextBox();

	void Init();

	std::string GetText() { return text; }

	void Update(SDL_Event* e);

	void Draw();

	void GetPosition(int* inputX, int* inputY);
	void GetSize(int* inputW, int* inputH);

	bool isSelected;

private:
	std::string text;

	int maxStrLength;

	SDL_Color backColor;//fill color of the text box
	SDL_Color boundColor;//outline color of the text box
	SDL_Color textColor;//color of the text entered

	SDL_Rect boxSquare;

	int x, y, width, height;

	SDL_Renderer* renderer;

	TTF_Font* defaultFont;

	float flickerLineCounter;//a counter to flash a line at the end of the current text to the user knows the box is selected
	float beginTick;

	void UpdateText(SDL_KeyboardEvent* key);

};