#pragma once
#include "SDL.h"
#include "SDL_TTF.h"
#include "UITextBox.h"
#include "UILabel.h"
#include "UISelectionBox.h"
#include "UIButton.h"
#include "Common.h"
#include "FileDialog.h"
#include "Animator.h"
#include "SoundManager.h"
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

	int frames;

private:
	SDL_Window *gameWindow;
	SDL_Renderer *renderer;
	string filename;

	SDL_Color uiColor;//UI Color
	SDL_Color uiOutlineColor;//UI Color

	//UI elements
	SDL_Rect bottomRect;//rectangle at bottom of screen
	SDL_Rect sideRect;//rectangle at side of screen
	SDL_Rect topRect;//rectangle at top of screen

	SDL_Rect testRect;//This is used as a test for the UI elements

	std::vector<UITextBox*> UITextBoxes;
	std::vector<UILabel*> UILabels;
	std::vector<UISelectionBox*> UISelectionBoxes;
	std::vector<UIButton*> UIButtons;
	std::vector<Animator*> Animators;


	TTF_Font* defaultFont;
	FileDialog* _FileDialog;
	SoundManager* _SoundManager;

};