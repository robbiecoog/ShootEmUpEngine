#include "FileDialog.h"



FileDialog::FileDialog()
{
	exportLines = vector<string>();
}

void FileDialog::GetOpenFile()
{
	char targetFile[MAX_PATH];
	OPENFILENAME file;
	ifstream dataFile;
	string data;

	ZeroMemory(&targetFile, sizeof(targetFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = targetFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 

	/*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
	system directory that contains the users most recently used documents
	OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
	if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&file))
	{
		cout << "You chose the file \ " << targetFile << "\"\n"; //Gets file user open name
		dataFile.open(targetFile);

		if (dataFile.is_open())
		{
			while (getline(dataFile, data))
			{
				cout << data << '\n';
			}
			dataFile.close();
		}
		else
		{
			cout << "Unable to open file";
		}
	}
	else
	{
		CheckErrors();
	}
}

void FileDialog::SaveFile()
{
	char saveFile[MAX_PATH];
	OPENFILENAME file;

	ZeroMemory(&saveFile, sizeof(saveFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = saveFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 
										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT;



	if (GetSaveFileNameA(&file))
	{
		ofstream fileToOpen;

		//check if '.proj' is added at the end of the filename
		std::string fileName = saveFile;
		if (fileName[fileName.size() - 1] == 'j' && fileName[fileName.size() - 2] == 'o' && fileName[fileName.size() - 3] == 'r' && fileName[fileName.size() - 4] == 'p' && fileName[fileName.size() - 5] == '.') {}
		else
		{
			fileName.append(".proj");
		}
		for (int i = 0; i < fileName.size(); i++) { saveFile[i] = fileName[i]; }

		fileToOpen.open(saveFile);
		fileToOpen << "Writing to file...\n";
		fileToOpen << "WOOHOOOOOOO \n";
		fileToOpen.close();
		cout << " You saved this file \ " << saveFile << "\"\n"; //Gets file user open name

	}
	else
	{
		CheckErrors();
	}
}

void FileDialog::PrepBGAudioExport(string name, string filePath)
{
	string BGAudioString = "background audio|" + name + filePath;
	exportLines.push_back(BGAudioString);
}

void FileDialog::PrepAudioExport(string name, string filePath)
{
	string audioString = "audio|" + name + filePath;
	exportLines.push_back(audioString);
}

void FileDialog::PrepBGExport(string fileDir, int layerNumber, float speed)
{
	string fullPath = SDL_GetBasePath(); fullPath += fileDir;
	SDL_Surface* temp = IMG_Load(fullPath.c_str());
	
	string BGSpriteString = "Background|" + fileDir + "|" + std::to_string(temp->clip_rect.x) + "|" + std::to_string(temp->clip_rect.y) + "|" + std::to_string(temp->clip_rect.h) + "|" + std::to_string(temp->clip_rect.w) + "|" + std::to_string(temp->clip_rect.h) + "|" + std::to_string(temp->clip_rect.w) + "|" + std::to_string(layerNumber) + "|" + std::to_string(speed);
	exportLines.push_back(BGSpriteString);

	SDL_FreeSurface(temp);
}

void FileDialog::PrepPlayerExport(int health, bool enabled, int shootPointX, int shootPointY, float shootingSpeed, SHOOTING_TYPE shootingType, string shotFilePath, SDL_Rect XY, string shotSoundName, float shotDamage, string deathSoundName, string shipFilePath, SDL_Rect destRect, float shipSpeed)
{
	//convert shooting point enum to string
	string shootingTypeNEW;
	switch (shootingType)
	{
	case Single:
		shootingTypeNEW = "Single";
		break;
	case Dual_Split:
		shootingTypeNEW = "Dual_Split";
		break;
	case Dual_Spread:
		shootingTypeNEW = "Dual_Spread";
		break;
	default:
		shootingTypeNEW = "NULLERR";
		break;
	}

	string fullPath = SDL_GetBasePath(); 
	if (shipFilePath[0] == 't') shipFilePath = shipFilePath.substr(1, 255);
	fullPath.append(shipFilePath);
	SDL_Surface* temp = IMG_Load(fullPath.c_str());
	SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);

	string playerString = "player|" + std::to_string(health) + "|" + ((enabled) ? "true" : "false") + "|" + std::to_string(shootPointX) + "|" + std::to_string(shootPointY) + "|" + std::to_string((int)shootingSpeed) + "|" + shootingTypeNEW + "|" + shotFilePath + "|" + std::to_string(XY.x) + "|" + std::to_string(XY.y) + "|" + std::to_string(XY.w) + "|" + std::to_string(XY.h) + "|" + shotSoundName + "|" + std::to_string((int)shotDamage) + "|" + "null" + "|" + deathSoundName + "|" + shipFilePath + "|" + std::to_string(temp->clip_rect.x) + "|" + std::to_string(temp->clip_rect.y) + "|" + std::to_string(temp->clip_rect.h) + "|" + std::to_string(temp->clip_rect.w) + "|" + std::to_string(destRect.h) + "|" + std::to_string(destRect.w) + "|" + std::to_string((int)shipSpeed);
	int asd = 0;
	exportLines.push_back(playerString);

	SDL_FreeSurface(temp);
}

void FileDialog::PrepWaveExport(int health, bool enabled, int shootPointX, int shootPointY, float shootingSpeed, SHOOTING_TYPE shootingType, string shotSpriteName, SDL_Rect shotDestRect, string shotSoundName, float shotDamage, float quantityFrom, float quantityTo, FORMATION formation, MOVE_PATTERN movePattern, float speedFrom, float speedTo, float crashDamage, float startPos, float killPoints, SDL_Rect eDestRect)
{
	//convert shooting point enum to string
	string shootingTypeNEW;
	string formationNEW;
	string patternNEW;
	switch (shootingType)
	{
	case Single:
		shootingTypeNEW = "Single";
		break;
	case Dual_Split:
		shootingTypeNEW = "Dual_Split";
		break;
	case Dual_Spread:
		shootingTypeNEW = "Dual_Spread";
		break;
	default:
		shootingTypeNEW = "NULLERR";
		break;
	}
	switch (formation)
	{
	case stright:
		formationNEW = "stright";
		break;
	case diaginalDown:
		formationNEW = "diaginalDown";
		break;
	case diaginalUp:
		formationNEW = "diaginalUp";
		break;
	case zigZagUp:
		formationNEW = "zigZagUp";
		break;
	case zigZagDown:
		formationNEW = "zigZagDown";
		break;

	default:
		shootingTypeNEW = "NULLERR";
		break;
	}
	switch (movePattern)
	{
	case slow:
		patternNEW = "slow";
		break;
	case fast:
		patternNEW = "fast";
		break;
	default:
		patternNEW = "NULLERR";
		break;
	}

	string enemyString = "wave|" + std::to_string(health) + "|" + ((enabled) ? "true" : "false") + "|" + std::to_string(shootPointX) + "|" + std::to_string(shootPointY) + "|" + std::to_string((int)shootingSpeed) + "|" + shootingTypeNEW + "|" + shotSpriteName + "|" + std::to_string(shotDestRect.x) + "|" + std::to_string(shotDestRect.y) + "|" + std::to_string(shotDestRect.h) + "|" + std::to_string(shotDestRect.w) + "|" + shotSoundName + "|" + std::to_string((int)shotDamage) + "|" + std::to_string((int)quantityFrom) + "|" + std::to_string((int)quantityTo) + "|" + formationNEW + "|" + patternNEW + "|" + std::to_string((int)speedFrom) + "|" + std::to_string((int)speedTo) + "|" + std::to_string((int)crashDamage) + "|" + std::to_string((int)startPos) + "|" + std::to_string((int)killPoints) + "|" + std::to_string(eDestRect.x) + "|" + std::to_string(eDestRect.y) + "|" + std::to_string(eDestRect.h) + "|" + std::to_string(eDestRect.w);
	exportLines.push_back(enemyString);
}

void FileDialog::ExportFile(string title, bool orientation)
{
	char saveFile[MAX_PATH];
	OPENFILENAME file;

	ZeroMemory(&saveFile, sizeof(saveFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = saveFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 
										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT;



	if (GetSaveFileNameA(&file))
	{
		ofstream fileToOpen;

		//check if '.proj' is added at the end of the filename
		std::string fileName = saveFile;

		for (int i = 0; i < fileName.size(); i++) { saveFile[i] = fileName[i]; }

		fileToOpen.open(saveFile);

		fileToOpen << "title|" << title << "\n";
		fileToOpen << "orintation|";
		if (orientation) {	fileToOpen << "true";}
		else{fileToOpen << "false"; }
		
		fileToOpen << "\n";

		for (unsigned int i = 0; i < exportLines.size(); i++) { fileToOpen << exportLines[i].c_str() << "\n"; }

		fileToOpen.close();
	}
	else
	{
		CheckErrors();
	}

}

string FileDialog::GetFile()
{
	char file[MAX_PATH];
	OPENFILENAME fileDialog;
	ifstream spriteFile;


	ZeroMemory(&file, sizeof(file));
	ZeroMemory(&fileDialog, sizeof(fileDialog));
	fileDialog.lStructSize = sizeof(fileDialog);
	fileDialog.hwndOwner = NULL; //window handler if needed
	fileDialog.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	fileDialog.lpstrFile = file; //file userloads
	fileDialog.nMaxFile = MAX_PATH;
	fileDialog.lpstrTitle = "Select a File..."; //Dialog File title 

										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	fileDialog.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&fileDialog))
	{
		cout << "You chose the file \ " << file << "\"\n"; //Gets file user open name
		spriteFile.open(file);
	}
	else
	{
		CheckErrors();
	}
	return file;
}

void FileDialog::CheckErrors()
{
	//this is where all the checks will be -  this will narrow any errors down to a user cancellation if none other errors occur.
	switch (CommDlgExtendedError())
	{
	default: cout << "You Cancelled.\n";
		//Dialog box could not be created - happens when dialog box call specifies an invalid window handle
	case CDERR_DIALOGFAILURE: cout << "CDERR_DIALOG FAILURE: 0xFFFF. Dialog Box could not be loaded."; break;
		//Dialog Box cant find a specified resource
	case CDERR_FINDRESFAILURE: cout << "CDERR_FINDRESFAILURE: 0x0006. Dialog Box failed to find a specific resource."; break;
		//Dialog Box function failed during initailisation. Suffiecient Memory not available.
	case CDERR_INITIALIZATION: cout << "CDERR_INITIALIZATION: 0x0002. Dialog Box failed to initalise."; break;
		//Dialog Box function failed to load the specified resource
	case CDERR_LOADRESFAILURE: cout << "CDERR_LOADRESFAILURE: 0x0007. Dialog Box failed to load specifed resource."; break;
		//Dialog Box failes to load a specific string
	case CDERR_LOADSTRFAILURE: cout << "CDERR_LOADSTRFAILURE: 0x0005. Dialog Box failed to load a specific string"; break;
	case CDERR_LOCKRESFAILURE: cout << "CDERR_LOCKRESFAILURE: 0x0008. Dialog Box failed to lock a specific resource"; break;
	case CDERR_MEMALLOCFAILURE: cout << "CDERR_MEMALLOCFAILURE: 0x0009. Dialog Box unable to allocate the memory associated with the handle"; break;
	case CDERR_MEMLOCKFAILURE: cout << "CDERR_MEMLOCKFAILURE: 0x000A. Dialog Box unable to lock the memory associated with a handle"; break;
	case CDERR_NOHINSTANCE: cout << "CDERR_NOHINSTANCE: 0x0004. The ENABLETEMPLATE flag was set but failed to provide a corresponding instance handle"; break;
	case CDERR_NOHOOK: cout << "CDERR_NOHOOK: 0x000B. Enable Hook flag was set in the flags member but you failed to provide a pointer to a corresponding hook procedure"; break;
	case CDERR_REGISTERMSGFAIL: cout << "CDERR_REGISTERMSGFAIL: 0x000C. Register Window Message function returned an error code when it was called by the dialog box function"; break;
	case CDERR_STRUCTSIZE: cout << "CDERR_STRUCTSIZE: 0x0001. IStructSize member of the intialisation structure for the corresponding common dialog box is invalid."; break;
		//Following error codes can be returned for the GetOpenFileName and GetSaveFileName fucntions.

		/*Buffer pointed to by the IPSTRFile member of OPENFILENAME structure is too small for the file name specified by user First 2 bytesof the PSTRFile buffer
		contain an integer value specifying the size required to recieve the full name in characters*/
	case FNERR_BUFFERTOOSMALL: cout << "FNERR_BUFFERTOOSMALL: 0x3003. buffer pointed to the file is too small."; break;
	case FNERR_INVALIDFILENAME: cout << "FNERR_INVALIDFILENAME: 0x3002. File name is invalid."; break;
		//Attempt to subclass a list box failed due to sufficent memory not being available.
	case FNERR_SUBCLASSFAILURE: cout << "FNERR_SUBCLASSFAILURE: 0x3001. Attempt to subclass a list box failed."; break;
	}
}
FileDialog::~FileDialog()
{
}
