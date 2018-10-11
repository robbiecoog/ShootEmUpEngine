#include "UITextBox.h"
#include <iostream>

UITextBox::UITextBox(int xPos, int yPos, int boxWidth, SDL_Renderer* gameRenderer)
{
	x = xPos;
	y = yPos;
	width = boxWidth;
	height = 25;
	text = "";

	renderer = gameRenderer;

	Init();
}

UITextBox::UITextBox(int xPos, int yPos, int boxWidth, std::string inputText, SDL_Renderer* gameRenderer)//allows the text box to start with text in it
{
	x = xPos;
	y = yPos;
	width = boxWidth;
	height = 25;

	text = inputText;

	renderer = gameRenderer;

	Init();
}

UITextBox::~UITextBox()
{
	delete (defaultFont);
}

void UITextBox::Init()
{
	//initialize back color
	backColor = { 255, 255, 255, 255 };
	//initialize outline color
	boundColor = { 130, 130, 130, 255 };
	//initialize text color
	textColor = { 0, 0, 0, 255 };

	//initialize the square that will be drawed(?) in
	boxSquare.x = x; boxSquare.y = y;
	boxSquare.h = height;
	boxSquare.w = width;

	//use width to calculate max text length
	maxStrLength = width / 10;

	//default isSelected to false as the text box will not be selected upon startup
	isSelected = false;

	//init the font type of this textbox
	defaultFont = TTF_OpenFont("Abel.ttf", 18);
	if (defaultFont == NULL)
	{
		std::cout << "Couldnt find file";
	}

	//begintick will initialise at the amount of ms that have passed since SDL starting.
	//we can then calculate if flickerLineCounter is a certain amount of ms above that to see if an amount of time has passed
	beginTick = SDL_GetTicks();

}

void UITextBox::Update(SDL_Event* e)
{
	if (isSelected == true)
	{
		switch (e->type)
		{
		case SDL_KEYDOWN:
			UpdateText(&e->key);
			break;
		}

		//add to counter
		flickerLineCounter += SDL_GetTicks();
		if (flickerLineCounter >= beginTick + 5000) { beginTick = SDL_GetTicks(); flickerLineCounter = beginTick; }

		std::cout << "LineCounter: " << flickerLineCounter << "\tbeginTick: " << beginTick << "\n";
	}



	//when updated, if any changes are made to the location, we need to ensure that the drawn box is also moved
	boxSquare.x = x; boxSquare.y = y;
}

void UITextBox::UpdateText(SDL_KeyboardEvent* key)
{
	if (key->keysym.sym == SDLK_BACKSPACE)
	{
		text = text.substr(0, text.size() - 1);
	}
	else if (key->keysym.sym == SDLK_SPACE)
	{
		if (text.size() < maxStrLength) { text = text + ' '; }
	}
	else if (key->keysym.sym == SDLK_LSHIFT || key->keysym.sym == SDLK_RSHIFT || key->keysym.sym == SDLK_TAB || key->keysym.sym == SDLK_RETURN)//stops letters being displayed when special keys are pressed
	{

	}
	else if (key->keysym.mod == KMOD_CAPS)//if caps lock is down
	{
		if (text.size() < maxStrLength) { text += (*SDL_GetKeyName(key->keysym.sym)); }
	}
	else
	{
		if (text.size() < maxStrLength) { text += (tolower(*SDL_GetKeyName(key->keysym.sym))); }
	}

	key = nullptr;
}

void UITextBox::GetPosition(int* inputX, int* inputY)
{
	*inputX = x;
	*inputY = y;
}

void UITextBox::GetSize(int* inputW, int* inputH)
{
	*inputW = width;
	*inputH = height;
}



void UITextBox::Draw()
{
	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);//set color to fill color
	SDL_RenderFillRect(renderer, &boxSquare);//Draw back of text box

	SDL_SetRenderDrawColor(renderer, boundColor.r, boundColor.g, boundColor.b, boundColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &boxSquare);//Draw outline of text box

	//SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);//set color to text color
	//draw the text
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);//create a surface using text & font values
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn
	//create a rectangle for the text to be drawn in, this will not be the rectangle used for drawing the text box itself.
	SDL_Rect textRect = { boxSquare.x + 3, boxSquare.y, (drawText != nullptr) ? drawText->w : 0, boxSquare.h };//input is: xpos, ypos, boxWidth, boxHeight
	SDL_RenderCopy(renderer, textTex, NULL, &textRect);

	//draw flickering line at the end of the typed string
	if (isSelected && flickerLineCounter < beginTick + 2500)
	{
		SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);//set color to fill color
		SDL_Rect line = { textRect.x + textRect.w, textRect.y + 3, 4, boxSquare.h - 6 };
		SDL_RenderFillRect(renderer, &line);//Draw outline of text box
	}
	SDL_FreeSurface(drawText);
	drawText = nullptr;
	SDL_DestroyTexture(textTex);
	textTex = nullptr;
	
}
