#include "GameObject.h"


GameObject::GameObject(std::string inputName, GameObjectType inputType, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer) : name(inputName), type(inputType), x(inputX), y(inputY), renderer(gameRenderer)
{
	texture = NULL;
	SDL_Surface* loadSurface = IMG_Load(imageDir.c_str());
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
	else
	{
		SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		
		texture = SDL_CreateTextureFromSurface(renderer, loadSurface);

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		sourceRect = loadSurface->clip_rect;
		destRect = sourceRect;

	}

	//adjust the start x and y positions to have the object spawn in the centre of the mouse
	destRect.x = x - (loadSurface->w / 2);
	destRect.y = y - (loadSurface->h / 2);
	width = destRect.w;
	height = destRect.h;

	//free old surface
	SDL_FreeSurface(loadSurface);
	loadSurface = nullptr;

	justDropped = true;


	isClicked = false;
	clickXOffset = 0;
	clickYOffset = 0;

	rotation = 0;
}

GameObject::~GameObject()
{

}

bool GameObject::IsUpdated()
{
	bool copy = isUpdated;
	if(isUpdated) isUpdated = false;

	return copy;
}

void GameObject::Update(SDL_Event* e)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (isClicked)
	{
		x = mouseX + clickXOffset;
		y = mouseY + clickYOffset;
		isUpdated = true;
	}

	modX = scrollBarOffset;

	if (justDropped)
	{
		x = x - destRect.w / 2;
		y = y - destRect.h / 2;

		justDropped = false;
		x = x - scrollBarOffset;
	}

	destRect.x = x + modX;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;
}

void GameObject::SetClicked(bool inputBool)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	clickXOffset = x - mouseX;
	clickYOffset = y - mouseY;

	isClicked = inputBool;
}


void GameObject::Draw(int hBarOffset)
{
	scrollBarOffset = hBarOffset;

	SDL_Rect offsetRect;//this rectangle will take into consideration how far along the scrollbar is.
	offsetRect = destRect;
	offsetRect.x = offsetRect.x + modX;

	if (isClicked)
	{
		SDL_SetTextureAlphaMod(texture, 70);
		//below code will draw a rect around the sprite when it is being dragged
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 130);
		SDL_Rect outlineRect = { destRect.x - 1, destRect.y - 1, destRect.w + 2, destRect.h + 2 };
		SDL_RenderDrawRect(renderer, &outlineRect);
	}
	else
	{
		SDL_SetTextureAlphaMod(texture, 255);
	}
	SDL_Point centre = { width/2, height/2 };
	SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &centre, SDL_RendererFlip::SDL_FLIP_NONE);


}