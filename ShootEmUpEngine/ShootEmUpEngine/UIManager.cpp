#include "UIManager.h"

UIManager::UIManager(SDL_Window *window, SDL_Renderer *gameRenderer) : gameWindow(window), renderer(gameRenderer)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;//elements will not always push to the exact edge of the screen, the border size is that threshhold

	//Initialize the rectangles for the UI
	sideRect = { screenWidth - (screenWidth / 4), 0, screenWidth / 4, screenHeight };
	bottomRect = { 0, (screenHeight - (screenHeight / 3)), screenWidth - (sideRect.w) + 1, screenHeight / 3 };
	topRect = { 0, 0, screenWidth - sideRect.w + 1, 30 };

	//init the font type of this textbox
	defaultFont = TTF_OpenFont("Abel.ttf", 20);
	if (defaultFont == NULL)
	{
		std::cout << "Couldnt find font file";
	}

	//initalize the colors to be used in the UI
	uiColor = { 100, 100, 100, 255 };
	uiOutlineColor = { 0, 0, 0, 255 };

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w/4), sideRect.y + 2, "Details", renderer, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->y + detailsLabel->height, "Object Name: ", renderer, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->y + objectNameLabel->height, (sideRect.w/2) - (borderSize*2), "", renderer, defaultFont);

	UILabel* xPosLabel = new UILabel(sideRect.x + borderSize, objectNameTextBox->y + objectNameTextBox->height + borderSize, "X: ", renderer, 0, defaultFont);
	UITextBox* xPosTextBox = new UITextBox(xPosLabel->x + xPosLabel->width, xPosLabel->y, objectNameTextBox->width / 3, "", renderer, defaultFont);

	UILabel* yPosLabel = new UILabel(xPosTextBox->x + xPosTextBox->width + borderSize, xPosLabel->y, "Y: ", renderer, 0, defaultFont);
	UITextBox* yPosTextBox = new UITextBox(yPosLabel->x + yPosLabel->width, yPosLabel->y, (sideRect.x + (sideRect.w / 2)) - (yPosLabel->x + yPosLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* widthLabel = new UILabel(xPosLabel->x, xPosLabel->y + yPosLabel->height + borderSize, "Width: ", renderer, 0, defaultFont);
	UITextBox* widthTextBox = new UITextBox(widthLabel->x + widthLabel->width, widthLabel->y, (sideRect.x + (sideRect.w / 2)) - (widthLabel->x + widthLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* heightLabel = new UILabel(widthLabel->x, widthLabel->y + widthLabel->height + borderSize, "Height: ", renderer, 0, defaultFont);
	UITextBox* heightTextBox = new UITextBox(heightLabel->x + heightLabel->width, heightLabel->y, (sideRect.x + (sideRect.w / 2)) - (heightLabel->x + heightLabel->width) - borderSize, "", renderer, defaultFont);

	//add labels to list
	UILabels.push_back(detailsLabel);
	UILabels.push_back(objectNameLabel);
	UILabels.push_back(xPosLabel);
	UILabels.push_back(yPosLabel);
	UILabels.push_back(widthLabel);
	UILabels.push_back(heightLabel);	

	//add textboxes to UIList
	UITextBoxes.push_back(objectNameTextBox);
	UITextBoxes.push_back(xPosTextBox);
	UITextBoxes.push_back(yPosTextBox);
	UITextBoxes.push_back(widthTextBox);
	UITextBoxes.push_back(heightTextBox);

	UIButton* AddObjectBtn = new UIButton(sideRect.x + (sideRect.w - sideRect.w/6), sideRect.y + borderSize, 20, 20, "+", renderer, defaultFont);
	UIButtons.push_back(AddObjectBtn);
	UIButton* DeleteObjectBtn = new UIButton(screenWidth - borderSize - AddObjectBtn->width, AddObjectBtn->y, 20, 20, "-", renderer, defaultFont);
	UIButtons.push_back(DeleteObjectBtn);
	UIButton* OpenFileDialogBtn = new UIButton(topRect.x, topRect.y, 60, 30, "Open", renderer, defaultFont);
	UIButtons.push_back(OpenFileDialogBtn);
	UIButton* SaveFileDialogBtn = new UIButton(OpenFileDialogBtn->x + OpenFileDialogBtn->width, topRect.y, 60, 30, "Save", renderer, defaultFont);
	UIButtons.push_back(SaveFileDialogBtn);
	UIButton* AddSpriteSheetBtn = new UIButton(700 + (sideRect.w - sideRect.w / 6), 250 + borderSize, 130, 20, "Add SpriteSheet", renderer, defaultFont);
	UIButtons.push_back(AddSpriteSheetBtn);

	//Create Selection Boxes
	UISelectionBox* objectSelectionBox = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->y + AddObjectBtn->height + borderSize, (sideRect.w / 2) - (borderSize*2), sideRect.h - AddObjectBtn->height - (borderSize * 3), renderer, defaultFont);
	objectSelectionBox->SetAlignment(0);

	//add Selection Boxes to SelectionBox list
	UISelectionBoxes.push_back(objectSelectionBox);
	//add items to the selection box
	objectSelectionBox->AddItem(new SelectionBoxItem{ "Object 1" });
	objectSelectionBox->AddItem(new SelectionBoxItem{ "Object 2" });
	objectSelectionBox->AddItem(new SelectionBoxItem{ "Object 3" });
	objectSelectionBox->AddItem(new SelectionBoxItem{ "Object 4" });

	UISelectionBox *levelSelectionBox = new UISelectionBox(bottomRect.x + borderSize, bottomRect.y + borderSize, (bottomRect.w / 2) - borderSize, bottomRect.h - (borderSize * 2), renderer, defaultFont);
	levelSelectionBox->SetAlignment(1);//set alignment to centre

	//add Selection Boxes to SelectionBox list
	UISelectionBoxes.push_back(levelSelectionBox);
	//add items to the selection box
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Forest Level" });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "City Level (Part 1)" });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "City Level (Part 2)" });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Underwater Level (Part 1)" });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Super Mega Awesome Secret Level 1" });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Level 6" });


}

UIManager::~UIManager()
{
	//iterate through textboxes
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->~UITextBox();}
	//iterate through labels
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->~UILabel(); }
	//iterate through UISelectionBox
	for (unsigned int i = 0; i < UISelectionBoxes.size(); i++) { UISelectionBoxes[i]->~UISelectionBox(); }
	//iterate through buyttons
	for (unsigned int i = 0; i < UIButtons.size(); i++) { UIButtons[i]->~UIButton(); }
}

void UIManager::Update(SDL_Event* e)
{
	//obtain mouse positions and store them
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);//store button press, and feed in references to x and y to store this as well. This is mouse position relative to the window. i.e. 0,0 is the top left of the active window

	switch (e->type)
	{
	case SDL_KEYDOWN:
		//if tab is pressed when a UITextBox is selected, tab to the next textbox
		if (e->key.keysym.sym == SDLK_TAB)
		{
			int setI = -1;
			bool shift = false;
			if (e->key.keysym.mod & KMOD_LSHIFT)//if shift is being held, we want to traverse the text boxes backwards
			{
				shift = true;
			}
			if (shift)
			{
				for (int i = 0; i < UITextBoxes.size(); i++)
				{
					if (UITextBoxes[i]->GetSelected())
					{
						UITextBoxes[i]->SetSelected(false);
						setI = (i != 0) ? i - 1 : UITextBoxes.size()-1;//if we aren't at the last textbox, flag the next textbox to be set to true after the loop. If we are at the last element, flag 0 to be set to true, as this will roll over
					}
				}
			}
			else
			{
				for (int i = 0; i < UITextBoxes.size(); i++)
				{
					if (UITextBoxes[i]->GetSelected())
					{
						UITextBoxes[i]->SetSelected(false);
						setI = (i != UITextBoxes.size() - 1) ? i + 1 : 0;//if we aren't at the last textbox, flag the next textbox to be set to true after the loop. If we are at the last element, flag 0 to be set to true, as this will roll over
					}
				}
			}

			if (setI > -1)//if it has been set
			{
				UITextBoxes[setI]->SetSelected(true);
			}
		}
		else if (e->key.keysym.sym == SDLK_UP)
		{
			_FileDialog->GetFile();
		}
		break;
	}

	//check buttons
	if (UIButtons[0]->CheckClick())//if + button is clicked
	{
		UISelectionBoxes[0]->AddItem(new SelectionBoxItem{ "New Item" });//add new item to the object list
	}
	if (UIButtons[2]->CheckClick())//if open file dialog button is clicked
	{
		_FileDialog->GetOpenFile();
	}
	if (UIButtons[3]->CheckClick())//if save dialog button is clicked
	{
		_FileDialog->SaveFile();
	}
	if (UIButtons[4]->CheckClick())//if add sprite sheet button is clicked
	{
		Animators.push_back(new Animator(renderer, gameWindow));
	}



	for (int i = 0; i < Animators.size(); i++)
	{
		Animators[i]->Update(e);
	}

	//for each UI element, we will check if the mouse is contained within this and will tell the object that it is selected if so.
	if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT))//if the left button is clicked
	{
		//everything that isnt clicked will need to be deselected, so only things that are clicked will be selected.
		for (unsigned int ii = 0; ii < UITextBoxes.size(); ii++)
		{
			UITextBoxes[ii]->SetSelected(false);
		}
		for (unsigned int i = 0; i < UITextBoxes.size(); i++)
		{
			int elementX = UITextBoxes[i]->x; int elementY = UITextBoxes[i]->y;
			int elementW, elementH; UITextBoxes[i]->GetSize(&elementW, &elementH);
			if (mouseX > elementX && mouseX < (elementX + elementW) && mouseY > elementY && mouseY < (elementY + elementH))
			{
				UITextBoxes[i]->SetSelected(true);
			}
		}

		//iterate through SelectionBoxes and pass the x and y coords of the mouse, so they can perform their own checks.
		SelectionBoxItem* objectDetails = nullptr;
		if (objectDetails = UISelectionBoxes[0]->CheckSelected(mouseX, mouseY))//if initialized
		{
			UITextBoxes[0]->SetText(objectDetails->name);
		}
		objectDetails = nullptr;

		UISelectionBoxes[1]->CheckSelected(mouseX, mouseY);
	}

	if (UISelectionBoxes[0]->GetSelectedItem() != nullptr && UISelectionBoxes[0]->GetSelectedItem()->name != UITextBoxes[0]->GetText())
	{
		UISelectionBoxes[0]->GetSelectedItem()->name = UITextBoxes[0]->GetText();
	}

	//iterate through UI & update
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->Update(e);}
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->Update(e); }
	for (unsigned int i = 0; i < UISelectionBoxes.size(); i++) { UISelectionBoxes[i]->Update(e); }
	for (unsigned int i = 0; i < UIButtons.size(); i++) { UIButtons[i]->Update(e); }



	try
	{
		if (UITextBoxes[1]->GetText() != "" && UITextBoxes[2]->GetText() != "" && UITextBoxes[3]->GetText() != "" && UITextBoxes[4]->GetText() != "")
			testRect = { stoi(UITextBoxes[1]->GetText()), stoi(UITextBoxes[2]->GetText()), stoi(UITextBoxes[3]->GetText()), stoi(UITextBoxes[4]->GetText()) };//update test rect
	}
	catch (const std::exception&){}

}

void UIManager::Draw()
{
	SDL_SetRenderDrawColor(renderer, uiColor.r, uiColor.g, uiColor.b, uiColor.a);//set color to fill color
	SDL_RenderFillRect(renderer, &bottomRect);//Draw bottom rectangle fill
	SDL_RenderFillRect(renderer, &sideRect);//draw side rectangle fill
	SDL_RenderFillRect(renderer, &topRect);//draw top rectangle fill
	
	SDL_SetRenderDrawColor(renderer, uiOutlineColor.r, uiOutlineColor.g, uiOutlineColor.b, uiOutlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &bottomRect);//Draw bottom rectangle outline
	SDL_RenderDrawRect(renderer, &sideRect);//Draw side rectangle outline
	SDL_RenderDrawRect(renderer, &topRect);//Draw top rectangle outline

	SDL_RenderFillRect(renderer, &testRect);//Draw test rectangle

	//NOTE: You can remove the below if you don't like how the UI looks regarding the smoothed borders between each UI box
	//create a quick rect to cover up the outline between the top bar and the side bar, and to cover up the bottom bar and side bar.
	SDL_SetRenderDrawColor(renderer, uiColor.r, uiColor.g, uiColor.b, uiColor.a);//set color to fill color
	SDL_Rect invisRect = { sideRect.x, sideRect.y + 1, 1, topRect.h - 2 };
	SDL_RenderFillRect(renderer, &invisRect);//draw
	invisRect = { bottomRect.x + bottomRect.w - 1, bottomRect.y + 1, 1, bottomRect.h - 2 };
	SDL_RenderFillRect(renderer, &invisRect);//draw
	SDL_SetRenderDrawColor(renderer, uiOutlineColor.r, uiOutlineColor.g, uiOutlineColor.b, uiOutlineColor.a);//set color to outline color
	invisRect = { sideRect.x + (sideRect.w / 2), 0, 1, sideRect.h };
	SDL_RenderFillRect(renderer, &invisRect);//draw


	//iterate through UI & Draw
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->Draw();}
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->Draw(); }
	for (unsigned int i = 0; i < UISelectionBoxes.size(); i++) { UISelectionBoxes[i]->Draw(); }
	for (unsigned int i = 0; i < UIButtons.size(); i++) { UIButtons[i]->Draw(); }
	for (unsigned int i = 0; i < Animators.size(); i++){ Animators[i]->Draw(); }
}