#include "UIManager.h"


UIManager::UIManager(SDL_Window *window, SDL_Renderer *gameRenderer) : gameWindow(window), renderer(gameRenderer)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	//Initialize the rectangles for the UI
	SideRect.x = screenWidth - (screenWidth / 4); SideRect.y = 0; SideRect.w = screenWidth / 4; SideRect.h = screenHeight;
	bottomRect.x = 0; bottomRect.y = screenHeight - (screenHeight / 3.6) + 1; bottomRect.w = screenWidth - (SideRect.w) + 1; bottomRect.h = screenHeight / 3.6;
	//initalize the colors to be used in the UI
	uiColor.r = 100; uiColor.g = 100; uiColor.b = 100; uiColor.a = 255;	
	uiOutlineColor.r = 0; uiOutlineColor.g = 0; uiOutlineColor.b = 0; uiOutlineColor.a = 255;

	//create the labels to be drawn on the UI
	UILabel *label1 = new UILabel(SideRect.x + (SideRect.w/4) - 10, SideRect.y, "Details", renderer, 1);
	UILabel *label2 = new UILabel(SideRect.x + (SideRect.w / 8) - 10, 30, "X: ", renderer, 0);
	UILabel *label3 = new UILabel(SideRect.x + (SideRect.w / 8) - 10, 60, "Y: ", renderer, 0);
	UILabels.push_back(label1);
	UILabels.push_back(label2);
	UILabels.push_back(label3);

	//Create the text boxes to be used
	UITextBox *textbox = new UITextBox(SideRect.x + (SideRect.w / 8), 30, 50, "0", renderer);//draw a text box at location (10, 10) with width of 30, with default text of "Hello"
	UITextBox *textbox2 = new UITextBox(SideRect.x + (SideRect.w / 8), 60, 50, "0", renderer);//draw a text box at location (10, 10) with width of 30, with default text of "Hello"

	//add textbox to UIList
	UITextBoxes.push_back(textbox);
	UITextBoxes.push_back(textbox2);

}

UIManager::~UIManager()
{
	//iterate through textboxes
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->~UITextBox();}
	//iterate through labels
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->~UILabel(); }
}

void UIManager::Update(SDL_Event* e)
{
	//obtain mouse positions and store them
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);//store button press, and feed in references to x and y to store this as well. This is mouse position relative to the window. i.e. 0,0 is the top left of the active window

	//for each UI element, we will check if the mouse is contained within this and will tell the object that it is selected if so.
	if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT))//if the left button is clicked
	{
		//everything that isnt clicked will need to be deselected, so only things that are clicked will be selected
		for (unsigned int ii = 0; ii < UITextBoxes.size(); ii++)
		{
			UITextBoxes[ii]->isSelected = false;
		}
		for (unsigned int i = 0; i < UITextBoxes.size(); i++)
		{
			int elementX, elementY; UITextBoxes[i]->GetPosition(&elementX, &elementY);
			int elementW, elementH; UITextBoxes[i]->GetSize(&elementW, &elementH);
			if (mouseX > elementX && mouseX < (elementX + elementW) && mouseY > elementY && mouseY < (elementY + elementH))
			{
				UITextBoxes[i]->isSelected = true;
			}
		}
	}

	//iterate through UI & update
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->Update(e);}
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->Update(e); }
}

void UIManager::Draw()
{
	SDL_SetRenderDrawColor(renderer, uiColor.r, uiColor.g, uiColor.b, uiColor.a);//set color to fill color
	SDL_RenderFillRect(renderer, &bottomRect);//Draw bottom rectangle fill
	SDL_RenderFillRect(renderer, &SideRect);//draw side rectangle fill
	SDL_SetRenderDrawColor(renderer, uiOutlineColor.r, uiOutlineColor.g, uiOutlineColor.b, uiOutlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &bottomRect);//Draw bottom rectangle outline
	SDL_RenderDrawRect(renderer, &SideRect);//Draw side rectangle outline

	//iterate through UI & Draw
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->Draw();}
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->Draw(); }
}