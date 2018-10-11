#pragma once
#include "SDL.h"
#include "UITextBox.h"
#include "UILabel.h"
#include <vector>
#include <iostream>

class UIManager
{
	//this manager will hold all UI elements and control interaction with the UI


public:
	UIManager(SDL_Window *window, SDL_Renderer *gameRenderer);//needs a handle to the game window to be able to calculate UI dimensions
	~UIManager();

	void Update(SDL_Event* e);

	void Draw();

private:

	SDL_Window *gameWindow;
	SDL_Renderer *renderer;

	SDL_Color uiColor;//UI Color
	SDL_Color uiOutlineColor;//UI Color

	//UI elements
	SDL_Rect bottomRect;//rectangle at bottom of screen
	SDL_Rect SideRect;//rectangle at side of screen

	std::vector<UITextBox*> UITextBoxes;
	std::vector<UILabel*> UILabels;

};