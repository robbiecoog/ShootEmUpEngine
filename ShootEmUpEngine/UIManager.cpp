#include "UIManager.h"

UIManager::UIManager(bool* gameRunning, SDL_Window *window, SDL_Renderer *gameRenderer) : gameIsRunning(gameRunning), gameWindow(window), renderer(gameRenderer)
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
		std::cout << "Couldnt find font file";

	//initalize the colors to be used in the UI
	uiColor = { 100, 100, 100, 255 };
	uiOutlineColor = { 0, 0, 0, 255 };

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w/4), sideRect.y + 2, "Details", renderer, 1, defaultFont);

	//add labels to list
	UILabels.push_back(detailsLabel);

	UIButton* AddObjectBtn = new UIButton(sideRect.x + (sideRect.w - sideRect.w/6), sideRect.y + borderSize, 20, 20, "+", renderer, defaultFont);
	defaultUIButtons.push_back(AddObjectBtn);
	UIButton* DeleteObjectBtn = new UIButton(screenWidth - borderSize - AddObjectBtn->width, AddObjectBtn->y, 20, 20, "-", renderer, defaultFont);
	defaultUIButtons.push_back(DeleteObjectBtn);

	//Open & Save buttons
	UIButton* OpenFileDialogBtn = new UIButton(topRect.x, topRect.y, 60, 30, "Open", renderer, defaultFont);
	defaultUIButtons.push_back(OpenFileDialogBtn);
	UIButton* SaveFileDialogBtn = new UIButton(OpenFileDialogBtn->x + OpenFileDialogBtn->width -1, topRect.y, 60, 30, "Save", renderer, defaultFont);
	defaultUIButtons.push_back(SaveFileDialogBtn);
	UIButton* ExportFileBtn = new UIButton(SaveFileDialogBtn->x + SaveFileDialogBtn->width - 1, topRect.y, 60, 30, "Export", renderer, defaultFont);
	defaultUIButtons.push_back(ExportFileBtn);

	//Create Selection Boxes
	UISelectionBox* objectSelectionBox = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->y + AddObjectBtn->height + borderSize, (sideRect.w / 2) - (borderSize*2), sideRect.h - AddObjectBtn->height - (borderSize * 3), renderer, defaultFont);
	objectSelectionBox->SetAlignment(0);

	//add Selection Boxes to SelectionBox list
	UISelectionBoxes.push_back(objectSelectionBox);

	UISelectionBox *levelSelectionBox = new UISelectionBox(bottomRect.x + borderSize, bottomRect.y + borderSize, (bottomRect.w / 2) - borderSize, bottomRect.h - (borderSize * 2), renderer, defaultFont);
	levelSelectionBox->SetAlignment(1);//set alignment to centre

	//add Selection Boxes to SelectionBox list
	UISelectionBoxes.push_back(levelSelectionBox);
	//add items to the selection box
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Forest Level", false});
	levelSelectionBox->AddItem(new SelectionBoxItem{ "City Level (Part 1)", false });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "City Level (Part 2)", false });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Underwater Level (Part 1)", false });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Super Mega Awesome Secret Level 1", false });
	levelSelectionBox->AddItem(new SelectionBoxItem{ "Level 6", false });


	//add Scrollbar to bottom of view panel
	UIScrollbar* scrollBar = new UIScrollbar(bottomRect.x, bottomRect.y - 20 + 1, bottomRect.w, 20, renderer);
	UIScrollbars.push_back(scrollBar);

	//add drag drop elements
	std::string imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIPlayer.jpg");
	UIDragDrop* playerDragDrop = new UIDragDrop("Player", bottomRect.x + (bottomRect.w/2) + borderSize, bottomRect.y + borderSize, 80, 80, imgDir, renderer, window);
	imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIEnemyWave.jpg");
	UIDragDrop* enemyWaveDragDrop = new UIDragDrop("Enemy", playerDragDrop->x + playerDragDrop->width + borderSize, playerDragDrop->y, 80, 80, imgDir, renderer, window);
	imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIBGMusic.jpg");
	UIDragDrop* backgroundMusicDragDrop = new UIDragDrop("BGMusic", enemyWaveDragDrop->x + enemyWaveDragDrop->width + borderSize, enemyWaveDragDrop->y, 80, 80, imgDir, renderer, window);

	UIDragDrops.push_back(playerDragDrop);
	UIDragDrops.push_back(enemyWaveDragDrop);
	UIDragDrops.push_back(backgroundMusicDragDrop);

	_FileDialog = new FileDialog();


}

UIManager::~UIManager()
{
	//iterate through textboxes
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->~UITextBox();}
	//iterate through labels
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->~UILabel(); }
	//iterate through SelectionBoxes
	for (unsigned int i = 0; i < UISelectionBoxes.size(); i++) { UISelectionBoxes[i]->~UISelectionBox(); }
	//iterate through buttons
	for (unsigned int i = 0; i < defaultUIButtons.size(); i++) { defaultUIButtons[i]->~UIButton(); }
	for (unsigned int i = 0; i < UIButtons.size(); i++) { UIButtons[i]->~UIButton(); }
	//iterate through scrollbars
	for (unsigned int i = 0; i < UIScrollbars.size(); i++) { UIScrollbars[i]->~UIScrollbar(); }
	//iterate through drag drop elements
	for (unsigned int i = 0; i < UIDragDrops.size(); i++) { UIDragDrops[i]->~UIDragDrop(); }
	//iterate through drag drop elements
	for (unsigned int i = 0; i < Animators.size(); i++) { Animators[i]->~Animator(); }
}

void UIManager::ShowFileDialog()
{
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
			//if shift is being held, we want to traverse the text boxes backwards
			if (e->key.keysym.mod & KMOD_LSHIFT){	shift = true;	}

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

			//prevents vector subscript from going out of range when deleting objects from object list
			if (setI > -1){	UITextBoxes[setI]->SetSelected(true);}
		}
		break;
	}

	//check buttons
	if (defaultUIButtons[1]->CheckClick())//if '-' button is clicked
	{
		UISelectionBoxes[0]->RemoveAt(UISelectionBoxes[0]->GetSelectedIndex());
	}
	else if (defaultUIButtons[2]->CheckClick())
	{
		_FileDialog->GetOpenFile();
	}
	else if (defaultUIButtons[3]->CheckClick())
	{
		_FileDialog->SaveFile();
	}
	else if (defaultUIButtons[4]->CheckClick())//Check Export button click
	{
		for (unsigned int i = 0; i < UISelectionBoxes[0]->GetItemCount(); i++)
		{
			SDL_Rect destRect;
			switch (UISelectionBoxes[0]->GetItem(i)->gameObject->type)
			{
			case GameObjectType::PLAYER:
				destRect = { UISelectionBoxes[0]->GetItem(i)->gameObject->x, UISelectionBoxes[0]->GetItem(i)->gameObject->y, UISelectionBoxes[0]->GetItem(i)->gameObject->width, UISelectionBoxes[0]->GetItem(i)->gameObject->height };
				_FileDialog->PrepPlayerExport(100, false, 0, 0, 1, SHOOTING_TYPE::Single, "null", SDL_Rect{ 0, 0, 0, 0 }, "null", 0, "null", UISelectionBoxes[0]->GetItem(i)->gameObject->spriteDir, destRect, 10);
			break;
			case GameObjectType::ENEMYWAVE:
				destRect = { UISelectionBoxes[0]->GetItem(i)->gameObject->x, UISelectionBoxes[0]->GetItem(i)->gameObject->y, UISelectionBoxes[0]->GetItem(i)->gameObject->width, UISelectionBoxes[0]->GetItem(i)->gameObject->height };
				_FileDialog->PrepWaveExport(10, false, 0, 0, 1, SHOOTING_TYPE::Single, "null", SDL_Rect{0, 0, 0, 0}, "null", 0, 1, 1, FORMATION::stright,MOVE_PATTERN::slow, 10, 10, 10, 50, 100, destRect);
			break;
			}
		}
		_FileDialog->ExportFile("My Game", true);
	}

	//for each UI element, we will check if the mouse is contained within this and will tell the object that it is selected if so.
	if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT))//if the left button is clicked
	{
		//everything that isnt clicked will need to be deselected, so only things that are clicked will be selected.
		for (unsigned int ii = 0; ii < UITextBoxes.size(); ii++)
			UITextBoxes[ii]->SetSelected(false);

		for (unsigned int i = 0; i < UITextBoxes.size(); i++)
		{
			int elementX = UITextBoxes[i]->x; 
			int elementY = UITextBoxes[i]->y;
			int elementW, elementH; 
			UITextBoxes[i]->GetSize(&elementW, &elementH);

			if (mouseX > elementX && mouseX < (elementX + elementW) && mouseY > elementY && mouseY < (elementY + elementH))	
				UITextBoxes[i]->SetSelected(true);
		}

		//iterate through SelectionBoxes and pass the x and y coords of the mouse, so they can perform their own checks.
		UISelectionBoxes[0]->CheckSelected(mouseX, mouseY);
		UISelectionBoxes[1]->CheckSelected(mouseX, mouseY);
	}

	SelectionBoxItem * temp = UISelectionBoxes[0]->GetSelectedItem();

	//iterate through UI & update
	for (unsigned int i = 0; i < UITextBoxes.size(); i++){UITextBoxes[i]->Update(e);}
	for (unsigned int i = 0; i < UILabels.size(); i++) { UILabels[i]->Update(e); }
	for (unsigned int i = 0; i < Animators.size(); i++) { Animators[i]->Update(e);}
	for (unsigned int i = 0; i < UIButtons.size(); i++) { UIButtons[i]->Update(e); }
	for (unsigned int i = 0; i < UISelectionBoxes.size(); i++) 
	{ 
		UISelectionBoxes[i]->Update(e); 

		if (temp != nullptr && temp->isGameObject)
		{
			//if user has typed in the text box, update the gameobject
			switch (temp->gameObject->type)
			{
			case GameObjectType::PLAYER:
				if (UISelectionBoxes[0]->NewSelected())
				{
					ClearObjectDetails();
					DisplayPlayerUI();
				}
				if (UITextBoxes[0]->IsUpdated()) {
					UISelectionBoxes[0]->GetSelectedItem()->gameObject->name = UITextBoxes[0]->GetText();
					UISelectionBoxes[0]->GetSelectedItem()->name = UISelectionBoxes[0]->GetSelectedItem()->gameObject->name;
				}//object name
				else if (UITextBoxes[1]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->x = std::atoi((UITextBoxes[1]->GetText().c_str())); }//x pos
				else if (UITextBoxes[2]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->y = std::atoi((UITextBoxes[2]->GetText().c_str())); }//y pos
				else if (UITextBoxes[3]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->width = std::atoi((UITextBoxes[3]->GetText().c_str())); }//width
				else if (UITextBoxes[4]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->height = std::atoi((UITextBoxes[4]->GetText().c_str())); }//height
				else if (UITextBoxes[5]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->rotation = std::atoi((UITextBoxes[5]->GetText().c_str())); }//rotation
				else if (UITextBoxes[6]->IsUpdated()) { if (UITextBoxes[6]->GetText() != "") for (unsigned int a = 0; a < Animators.size(); a++) { Animators[a]->SetFrames(std::atoi(UITextBoxes[6]->GetText().c_str())); } }
				else if (UIButtons[0]->CheckClick()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->SetNewImgFromFile(_FileDialog->GetFile()); }//Load sprite
				else if (UIButtons[1]->CheckClick()) Animators.push_back(new Animator(renderer, gameWindow));//Create Animator
				else if (UIButtons[2]->CheckClick()) SoundManager::getInstance()->PlaySFX(_FileDialog->GetFile());//Add SFX button
				
				break;
			case GameObjectType::ENEMYWAVE:
				if (UISelectionBoxes[0]->NewSelected())
				{
					ClearObjectDetails();
					DisplayEnemyUI();
				}
				if (UITextBoxes[0]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->name = UITextBoxes[0]->GetText(); UISelectionBoxes[0]->GetSelectedItem()->name = UISelectionBoxes[0]->GetSelectedItem()->gameObject->name; }//object name
				else if (UITextBoxes[1]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->x = std::atoi((UITextBoxes[1]->GetText().c_str())); }//x pos
				else if (UITextBoxes[2]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->y = std::atoi((UITextBoxes[2]->GetText().c_str())); }//y pos
				else if (UITextBoxes[3]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->width = std::atoi((UITextBoxes[3]->GetText().c_str())); }//width
				else if (UITextBoxes[4]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->height = std::atoi((UITextBoxes[4]->GetText().c_str())); }//height
				else if (UITextBoxes[5]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->gameObject->rotation = std::atoi((UITextBoxes[5]->GetText().c_str())); }//height
				else if (UITextBoxes[6]->IsUpdated()) { if (UITextBoxes[6]->GetText() != "") for (unsigned int a = 0; a < Animators.size(); a++) { Animators[a]->SetFrames(std::atoi(UITextBoxes[6]->GetText().c_str())); } }//animator frames
				else if (UIButtons[0]->CheckClick()) { Animators.push_back(new Animator(renderer, gameWindow)); }
				else if (UIButtons[1]->CheckClick()) SoundManager::getInstance()->PlaySFX(_FileDialog->GetFile());//Add SFX button
				break;
			case GameObjectType::BACKGROUNDMUSIC:
				if (UISelectionBoxes[0]->NewSelected()) {
					ClearObjectDetails();
					DisplayBGMusicUI();
				}
				if (UITextBoxes[0]->IsUpdated()) { UISelectionBoxes[0]->GetSelectedItem()->name = UITextBoxes[0]->GetText(); }//Object name
				else if (UIButtons[1]->CheckClick()) SoundManager::getInstance()->PlayMusic(_FileDialog->GetFile());//play music button
				break;
			}
		}
	}
	for (unsigned int i = 0; i < defaultUIButtons.size(); i++) { defaultUIButtons[i]->Update(e); }

	if (temp && temp->isGameObject && temp->gameObject->IsUpdated())
	{
		UpdateGOUI();
	}

	
	for (unsigned int i = 0; i < UIScrollbars.size(); i++) { UIScrollbars[i]->Update(e); }
	//check through dragdrops
	for (unsigned int i = 0; i < UIDragDrops.size(); i++)
	{
		SDL_Rect gameScreenRect = {0, topRect.y+topRect.h, 0 + sideRect.x, bottomRect.y - (topRect.y + topRect.h)};
		UIDragDrops[i]->Update(e, gameScreenRect);
		//HasDropped returns a Selectionboxitem pointer that is only null if nothing has dropped. If it isnt null, the user has dropped an element onto the screen.
		if (temp = UIDragDrops[i]->HasDropped())//if selectionboxitem that is returned is not null
		{
			UISelectionBoxes[0]->AddItem(temp);//add new item to the object list
		}
	}
 
}

void UIManager::ClearObjectDetails()
{
	//wipe all UI text boxes from the object details section
	for (int i = 0; i < UILabels.size(); i++)
	{
		delete UILabels[i];
	}
	UILabels.clear();
	for (int i = 0; i < UITextBoxes.size(); i++)
	{
		delete UITextBoxes[i];
		//UITextBoxes.erase(UITextBoxes.begin() + i);
	}
	UITextBoxes.clear();

	for (int i = 0; i <= UIButtons.size() && UIButtons.size() > 0; i++)
	{
		delete UIButtons[i];
		UIButtons.erase(UIButtons.begin() + i);
		i--;
	}
	//UIButtons.erase(UIButtons.begin() + 5, UIButtons.begin() + UIButtons.size());
}

void UIManager::DisplayPlayerUI()
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->y + detailsLabel->height, "Object Name: ", renderer, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->y + objectNameLabel->height, (sideRect.w / 2) - (borderSize * 2), "", renderer, defaultFont);

	UILabel* xPosLabel = new UILabel(sideRect.x + borderSize, objectNameTextBox->y + objectNameTextBox->height + borderSize, "X: ", renderer, 0, defaultFont);
	UITextBox* xPosTextBox = new UITextBox(xPosLabel->x + xPosLabel->width, xPosLabel->y, objectNameTextBox->width / 3, "", renderer, defaultFont);

	UILabel* yPosLabel = new UILabel(xPosTextBox->x + xPosTextBox->width + borderSize, xPosLabel->y, "Y: ", renderer, 0, defaultFont);
	UITextBox* yPosTextBox = new UITextBox(yPosLabel->x + yPosLabel->width, yPosLabel->y, (sideRect.x + (sideRect.w / 2)) - (yPosLabel->x + yPosLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* widthLabel = new UILabel(xPosLabel->x, xPosLabel->y + yPosLabel->height + borderSize, "Width: ", renderer, 0, defaultFont);
	UITextBox* widthTextBox = new UITextBox(widthLabel->x + widthLabel->width, widthLabel->y, (sideRect.x + (sideRect.w / 2)) - (widthLabel->x + widthLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* heightLabel = new UILabel(widthLabel->x, widthLabel->y + widthLabel->height + borderSize, "Height: ", renderer, 0, defaultFont);
	UITextBox* heightTextBox = new UITextBox(heightLabel->x + heightLabel->width, heightLabel->y, (sideRect.x + (sideRect.w / 2)) - (heightLabel->x + heightLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* rotationLabel = new UILabel(objectNameLabel->x, heightLabel->y + heightLabel->height, "Rotation: ", renderer, 1, defaultFont);
	UITextBox* rotationTextBox = new UITextBox(sideRect.x + borderSize, rotationLabel->y + rotationLabel->height, objectNameTextBox->width, "", renderer, defaultFont);

	UILabel* spriteLabel = new UILabel(sideRect.x + borderSize, rotationTextBox->y + rotationTextBox->height + borderSize + borderSize, "Sprite:", renderer, 0, defaultFont);
	UIButton* spriteButton = new UIButton(spriteLabel->x + spriteLabel->width + borderSize, spriteLabel->y, (sideRect.x + sideRect.w/2) - (spriteLabel->x + spriteLabel->width + borderSize) - borderSize, spriteLabel->height, "Load", renderer, defaultFont);

	UILabel* framesLabel = new UILabel(spriteLabel->x, spriteLabel->y + spriteLabel->height + borderSize, "Frames: ", renderer, 0, defaultFont);
	UITextBox* framesTextBox = new UITextBox(framesLabel->x + framesLabel->width, framesLabel->y, (sideRect.x + (sideRect.w / 2)) - (framesLabel->x + framesLabel->width) - borderSize, "", renderer, defaultFont);

	UIButton* AddSpriteSheetBtn = new UIButton(framesLabel->x, framesLabel->y + framesLabel->height + borderSize, rotationTextBox->width, rotationTextBox->height, "Add SpriteSheet", renderer, defaultFont);
	UIButton* PlaySFXBtn = new UIButton(AddSpriteSheetBtn->x, AddSpriteSheetBtn->y + AddSpriteSheetBtn->height + borderSize, AddSpriteSheetBtn->width, AddSpriteSheetBtn->height, "Play SFX", renderer, defaultFont);

	//add labels to list
	UILabels.push_back(detailsLabel);
	UILabels.push_back(objectNameLabel);
	UILabels.push_back(xPosLabel);
	UILabels.push_back(yPosLabel);
	UILabels.push_back(widthLabel);
	UILabels.push_back(heightLabel);
	UILabels.push_back(rotationLabel);
	UILabels.push_back(spriteLabel);
	UILabels.push_back(framesLabel);

	//add textboxes to UIList
	UITextBoxes.push_back(objectNameTextBox);
	UITextBoxes.push_back(xPosTextBox);
	UITextBoxes.push_back(yPosTextBox);
	UITextBoxes.push_back(widthTextBox);
	UITextBoxes.push_back(heightTextBox);
	UITextBoxes.push_back(rotationTextBox);
	UITextBoxes.push_back(framesTextBox);

	//add buttons
	UIButtons.push_back(spriteButton);
	UIButtons.push_back(AddSpriteSheetBtn);
	UIButtons.push_back(PlaySFXBtn);

	//populate the new areas with the relevant info from the gameobjects
	UITextBoxes[0]->SetText(UISelectionBoxes[0]->GetSelectedItem()->gameObject->name);//object name text box
	UITextBoxes[1]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->x));//x pos text box
	UITextBoxes[2]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->y));//y pos text box
	UITextBoxes[3]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->width));//width text box
	UITextBoxes[4]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->height));//height text box
	UITextBoxes[5]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->rotation));//rotation text box
}

void UIManager::DisplayEnemyUI()
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->y + detailsLabel->height, "Object Name: ", renderer, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->y + objectNameLabel->height, (sideRect.w / 2) - (borderSize * 2), "", renderer, defaultFont);

	UILabel* xPosLabel = new UILabel(sideRect.x + borderSize, objectNameTextBox->y + objectNameTextBox->height + borderSize, "X: ", renderer, 0, defaultFont);
	UITextBox* xPosTextBox = new UITextBox(xPosLabel->x + xPosLabel->width, xPosLabel->y, objectNameTextBox->width / 3, "", renderer, defaultFont);

	UILabel* yPosLabel = new UILabel(xPosTextBox->x + xPosTextBox->width + borderSize, xPosLabel->y, "Y: ", renderer, 0, defaultFont);
	UITextBox* yPosTextBox = new UITextBox(yPosLabel->x + yPosLabel->width, yPosLabel->y, (sideRect.x + (sideRect.w / 2)) - (yPosLabel->x + yPosLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* widthLabel = new UILabel(xPosLabel->x, xPosLabel->y + yPosLabel->height + borderSize, "Width: ", renderer, 0, defaultFont);
	UITextBox* widthTextBox = new UITextBox(widthLabel->x + widthLabel->width, widthLabel->y, (sideRect.x + (sideRect.w / 2)) - (widthLabel->x + widthLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* heightLabel = new UILabel(widthLabel->x, widthLabel->y + widthLabel->height + borderSize, "Height: ", renderer, 0, defaultFont);
	UITextBox* heightTextBox = new UITextBox(heightLabel->x + heightLabel->width, heightLabel->y, (sideRect.x + (sideRect.w / 2)) - (heightLabel->x + heightLabel->width) - borderSize, "", renderer, defaultFont);

	UILabel* rotationLabel = new UILabel(objectNameLabel->x, heightLabel->y + heightLabel->height, "Rotation: ", renderer, 1, defaultFont);
	UITextBox* rotationTextBox = new UITextBox(sideRect.x + borderSize, rotationLabel->y + rotationLabel->height, objectNameTextBox->width, "", renderer, defaultFont);

	UILabel* framesLabel = new UILabel(sideRect.x + borderSize, rotationTextBox->y + rotationTextBox->height + borderSize, "Frames: ", renderer, 0, defaultFont);
	UITextBox* framesTextBox = new UITextBox(framesLabel->x + framesLabel->width, framesLabel->y, (sideRect.x + (sideRect.w / 2)) - (framesLabel->x + framesLabel->width) - borderSize, "", renderer, defaultFont);

	UIButton* AddSpriteSheetBtn = new UIButton(framesLabel->x, framesLabel->y + framesLabel->height + borderSize, rotationTextBox->width, rotationTextBox->height, "Add SpriteSheet", renderer, defaultFont);
	UIButton* PlaySFXBtn = new UIButton(AddSpriteSheetBtn->x, AddSpriteSheetBtn->y + AddSpriteSheetBtn->height + borderSize, AddSpriteSheetBtn->width, AddSpriteSheetBtn->height, "Play SFX", renderer, defaultFont);

	//add labels to list
	UILabels.push_back(detailsLabel);
	UILabels.push_back(objectNameLabel);
	UILabels.push_back(xPosLabel);
	UILabels.push_back(yPosLabel);
	UILabels.push_back(widthLabel);
	UILabels.push_back(heightLabel);
	UILabels.push_back(rotationLabel);
	UILabels.push_back(framesLabel);

	//add textboxes to UIList
	UITextBoxes.push_back(objectNameTextBox);
	UITextBoxes.push_back(xPosTextBox);
	UITextBoxes.push_back(yPosTextBox);
	UITextBoxes.push_back(widthTextBox);
	UITextBoxes.push_back(heightTextBox);
	UITextBoxes.push_back(rotationTextBox);
	UITextBoxes.push_back(framesTextBox);

	UIButtons.push_back(AddSpriteSheetBtn);
	UIButtons.push_back(PlaySFXBtn);


	//populate the new areas with the relevant info from the gameobjects
	UITextBoxes[0]->SetText(UISelectionBoxes[0]->GetSelectedItem()->gameObject->name);//object name text box
	UITextBoxes[1]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->x));//x pos text box
	UITextBoxes[2]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->y));//y pos text box
	UITextBoxes[3]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->width));//width text box
	UITextBoxes[4]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->height));//height text box
	UITextBoxes[5]->SetText(std::to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->rotation));//rotation text box
}

void UIManager::DisplayBGMusicUI()
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->y + detailsLabel->height, "Object Name: ", renderer, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->y + objectNameLabel->height, (sideRect.w / 2) - (borderSize * 2), "", renderer, defaultFont);

	UILabel* filePathLabel = new UILabel(detailsLabel->x, objectNameTextBox->y + objectNameTextBox->height + borderSize, "File Path:", renderer, 1, defaultFont);
	UITextBox* filePathTextBox = new UITextBox(sideRect.x + borderSize, filePathLabel->y + filePathLabel->height + borderSize, (sideRect.w/2) - borderSize - borderSize, renderer, defaultFont);

	UIButton* GetFilePathButton = new UIButton(filePathTextBox->x, filePathTextBox->y + filePathTextBox->height + borderSize, filePathTextBox->width/2 - borderSize, filePathTextBox->height, "Search..", renderer, defaultFont);
	UIButton* PlayMusicBtn = new UIButton(GetFilePathButton->x, GetFilePathButton->y + GetFilePathButton->height + borderSize, objectNameTextBox->width, objectNameTextBox->height, "Play Music", renderer, defaultFont);

	UILabels.push_back(detailsLabel);
	UILabels.push_back(objectNameLabel);
	UILabels.push_back(filePathLabel);

	UITextBoxes.push_back(objectNameTextBox);
	UITextBoxes.push_back(filePathTextBox);
	
	UIButtons.push_back(GetFilePathButton);
	UIButtons.push_back(PlayMusicBtn);

	UITextBoxes[0]->SetText(UISelectionBoxes[0]->GetSelectedItem()->name);

}

void UIManager::UpdateGOUI()
{
	switch (UISelectionBoxes[0]->GetSelectedItem()->gameObject->type)
	{
	case GameObjectType::PLAYER:
		UITextBoxes[0]->SetText(UISelectionBoxes[0]->GetSelectedItem()->gameObject->name);
		UITextBoxes[1]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->x));
		UITextBoxes[2]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->y));
		UITextBoxes[3]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->width));
		UITextBoxes[4]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->height));
		UITextBoxes[5]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->rotation));
	break;
	case GameObjectType::ENEMYWAVE:
		UITextBoxes[0]->SetText(UISelectionBoxes[0]->GetSelectedItem()->gameObject->name);
		UITextBoxes[1]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->x));
		UITextBoxes[2]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->y));
		UITextBoxes[3]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->width));
		UITextBoxes[4]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->height));
		UITextBoxes[5]->SetText(to_string(UISelectionBoxes[0]->GetSelectedItem()->gameObject->rotation));
	break;
	}

}

void UIManager::Draw()
{
	//draw gameobjects under the UI
	for (int i = 0; i < UISelectionBoxes[0]->GetItemCount(); i++)
	{
		UISelectionBoxes[0]->GetItem(i)->gameObject->Draw(UIScrollbars[0]->GetValue());
	}


	SDL_SetRenderDrawColor(renderer, uiColor.r, uiColor.g, uiColor.b, uiColor.a);//set color to fill color
	SDL_RenderFillRect(renderer, &bottomRect);//Draw bottom rectangle fill
	SDL_RenderFillRect(renderer, &sideRect);//draw side rectangle fill
	SDL_RenderFillRect(renderer, &topRect);//draw top rectangle fill
	
	SDL_SetRenderDrawColor(renderer, uiOutlineColor.r, uiOutlineColor.g, uiOutlineColor.b, uiOutlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &bottomRect);//Draw bottom rectangle outline
	SDL_RenderDrawRect(renderer, &sideRect);//Draw side rectangle outline
	SDL_RenderDrawRect(renderer, &topRect);//Draw top rectangle outline

	//NOTE: You can remove the below if you don't like how the UI looks regarding the 'smoothed' borders between each UI box (removing the below will add black bars between each section of the UI)
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
	for (unsigned int i = 0; i < defaultUIButtons.size(); i++) { defaultUIButtons[i]->Draw(); }
	for (unsigned int i = 0; i < UIButtons.size(); i++) { UIButtons[i]->Draw(); }
	for (unsigned int i = 0; i < UIScrollbars.size(); i++) { UIScrollbars[i]->Draw(); }
	for (unsigned int i = 0; i < UIDragDrops.size(); i++) { UIDragDrops[i]->Draw(); }
	for (unsigned int i = 0; i < Animators.size(); i++) { Animators[i]->Draw(); }
}