#include "UILabel.h"

UILabel::UILabel(int inputX, int inputY, std::string inputText, SDL_Renderer* gamerenderer, int inputAlignment)//alignment: 0=left, 1=center
{
	x = inputX;
	y = inputY;
	text = inputText;
	renderer = gamerenderer;

	textColor = { 0, 0, 0, 255 };

	//init the font type of this textbox
	defaultFont = TTF_OpenFont("Abel.ttf", 18);
	if (defaultFont == NULL)
	{
		std::cout << "Couldnt find file";
	}

	alignment = inputAlignment;
}

UILabel::~UILabel()
{

}

void UILabel::Update(SDL_Event* e)
{

}

void UILabel::Draw()
{
	SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);//set color to fill color

	//draw the text
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);//create a surface using text & font values
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn
	//create a rectangle for the text to be drawn in, this will not be the rectangle used for drawing the text box itself.
	if (alignment == 0) { SDL_Rect textRect = { x, y, (drawText != nullptr) ? drawText->w : 0, 25 }; };//input is: xpos, ypos, boxWidth, boxHeight
	if (alignment == 1) { SDL_Rect textRect = { (drawText != nullptr) ? (x - (drawText->w/2)) : 0, y, (drawText != nullptr) ? drawText->w : 0, 25 }; };//input is: xpos, ypos, boxWidth, boxHeight
	SDL_Rect textRect = { x - (text.size() * 4) + 3, y, text.size() * 8, 25 };//input is: xpos, ypos, boxWidth, boxHeight
	SDL_RenderCopy(renderer, textTex, NULL, &textRect);


	SDL_FreeSurface(drawText);
	drawText = nullptr;
	SDL_DestroyTexture(textTex);
	textTex = nullptr;
}