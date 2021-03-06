#include "UITextBox.h"
#include <iostream>

UITextBox::UITextBox(int xPos, int yPos, int boxWidth, SDL_Renderer* gameRenderer, TTF_Font* font)
{
	x = xPos;
	y = yPos;
	width = boxWidth;
	
	renderer = gameRenderer;

	defaultFont = font;

	Init();

	text = "";
}

UITextBox::UITextBox(int xPos, int yPos, int boxWidth, std::string inputText, SDL_Renderer* gameRenderer, TTF_Font* font)//allows the text box to start with text in it
{
	x = xPos;
	y = yPos;
	width = boxWidth;

	renderer = gameRenderer;

	defaultFont = font;

	Init();

	text = inputText;
}

UITextBox::~UITextBox()
{
	delete (defaultFont);
}

void UITextBox::Init()
{
	//init height value
	text = "a";//give this some default text to use to ensure that drawText has a size, this is replaced after Init with any initialized text anyway.
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);//surface uses the font to define it's size, so we use this to define our height
	height = drawText->h;


	//initialize back color
	backColor = { 70, 70, 70, 255 };
	//initialize outline color
	boundColor = { 0, 0, 0, 255 };
	//initialize text color
	textColor = { 200, 200, 200, 255 };

	//initialize the square that will be drawed(?) in
	boxSquare.x = x; boxSquare.y = y;
	boxSquare.h = height;
	boxSquare.w = width;

	//default isSelected to false as the text box will not be selected upon startup
	isSelected = false;

	timeCounter = 0;//used for counting the amount of MS that have passed
	flashingLineTimer = 500;//this is the amount of MS that will pass between each flick (on..off) of the black line that appears when the text box is selected

	deltaTime = 0;//init the delta time value
	lastFrameTick = SDL_GetTicks();//init value of what the last frame's tick number was so we can calculate deltatime appropriately later

	SDL_FreeSurface(drawText);
}

void UITextBox::SetSelected(bool input)
{
	isSelected = input;
	if (input == true)
	{
		timeCounter = 0;
	}
}

void UITextBox::Update(SDL_Event* e)
{
	float currentFrameTick = SDL_GetTicks();
	deltaTime = currentFrameTick - lastFrameTick;
	timeCounter += deltaTime;

	if (isSelected == true)
	{
		UpdateText(e);

		if (timeCounter > flashingLineTimer * 2)
		{
			timeCounter = 0;
		}

		backColor = { 75, 75, 75, 255 };
	}
	else
	{
		backColor = { 70, 70, 70, 255 };
	}



	lastFrameTick = SDL_GetTicks();
	//when updated, if any changes are made to the location, we need to ensure that the drawn box is also moved
	boxSquare.x = x; boxSquare.y = y;
}

void UITextBox::SetText(std::string input)
{
	text = input;
}

void UITextBox::UpdateText(SDL_Event* e)
{

	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.sym == SDLK_BACKSPACE)
		{
			text = text.substr(0, text.size() - 1);
		}
		else if (e->key.keysym.sym == SDLK_SPACE)
		{
			text = text + ' ';
		}
		else
		{
			std::string fullKey = SDL_GetKeyName(e->key.keysym.sym);
			if (fullKey.size() <= 1)//if the string that is received is anything longer than a single character, it will be something like "Left Shift" when the left shift button is pressed. We don't want this, so we will filter these events out
			{
				//check for numbers
				if (e->key.keysym.mod & KMOD_SHIFT && (fullKey[0] == '1' || fullKey[0] == '2' || fullKey[0] == '3' || fullKey[0] == '4' || fullKey[0] == '5' || fullKey[0] == '6' || fullKey[0] == '7' || fullKey[0] == '8' || fullKey[0] == '9' || fullKey[0] == '0' || fullKey[0] == ',' || fullKey[0] == '.' || fullKey[0] == '/' || fullKey[0] == ';' || fullKey[0] == '\'' || fullKey[0] == '[' || fullKey[0] == ']' || fullKey[0] == '\\') || fullKey[0] == '`')
				{
					if (*SDL_GetKeyName(e->key.keysym.sym) == '1') text += '!';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '2') text += '@';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '3') text += '#';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '4') text += '$';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '5') text += '%';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '6') text += '^';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '7') text += '&';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '8') text += '*';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '9') text += '(';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '0') text += ')';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == ',') text += '<';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '.') text += '>';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '/') text += '?';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == ';') text += ':';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '\'') text += '"';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '[') text += '{';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == ']') text += '}';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '\\') text += '|';
					else if (*SDL_GetKeyName(e->key.keysym.sym) == '`') text += '~';
				}
				else if (e->key.keysym.mod & KMOD_CAPS)
				{
					if (e->key.keysym.mod & KMOD_SHIFT)
					{
						text += tolower(*SDL_GetKeyName(e->key.keysym.sym));
					}
					else
					{
						text += toupper(*SDL_GetKeyName(e->key.keysym.sym));
					}
				}
				else
				{
					if (e->key.keysym.mod & KMOD_SHIFT)
					{
						text += toupper(*SDL_GetKeyName(e->key.keysym.sym));
					}
					else
					{
						text += tolower(*SDL_GetKeyName(e->key.keysym.sym));
					}
				}


			}
			SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);
			//We can now check whether the new text has pushed the text over the line. If this is the case, remove the new letter
			if (drawText != nullptr && drawText->w > boxSquare.w - 5)
			{
				text = text.substr(0, text.size() - 1);
			}
			SDL_FreeSurface(drawText);
			drawText = nullptr;
		}
	}
	else if (e->type == SDL_KEYUP)
	{

	}



	e = nullptr;
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

	//draw the text
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);//create a surface using text & font values
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn
	//create a rectangle for the text to be drawn in, this will not be the rectangle used for drawing the text box itself.
	SDL_Rect textRect = { boxSquare.x + 3, boxSquare.y, (drawText != nullptr) ? drawText->w : 0, boxSquare.h };//input is: xpos, ypos, boxWidth, boxHeight
	SDL_RenderCopy(renderer, textTex, NULL, &textRect);

	//draw flickering line at the end of the typed string
	if (isSelected && timeCounter < flashingLineTimer)
	{
		SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);//set color to fill color
		SDL_Rect line = { textRect.x + textRect.w, textRect.y + 3, 4, boxSquare.h - 6 };
		SDL_RenderFillRect(renderer, &line);
	}
	SDL_FreeSurface(drawText);
	drawText = nullptr;
	SDL_DestroyTexture(textTex);
	textTex = nullptr;
	
}
