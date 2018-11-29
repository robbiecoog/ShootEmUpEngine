#pragma once

#include "SDL_image.h"
#include "SDL.h"
#include <string>
#include <iostream>
#include "enums.h"
#include "Animator.h"

class GameObject
{
public:
	GameObject(std::string inputName, GameObjectType inputType, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer);
	~GameObject();

	void Update(SDL_Event* e);

	void Draw(int hBarOffset);

	GameObjectType type;

	bool IsUpdated();

	bool SetNewImgFromFile(std::string fileDir);

	int x, y, modX, width, height, rotation;
	SDL_Rect destRect;

	void SetClicked(bool inputBool);

	bool isClicked;

	std::string name;
	std::string spriteDir;

private:
	
	SDL_Texture* texture;

	SDL_Renderer* renderer;
	SDL_Rect sourceRect;
	

	
	int clickXOffset;
	int clickYOffset;

	bool justDropped;
	bool isUpdated;

	int scrollBarOffset;

};
