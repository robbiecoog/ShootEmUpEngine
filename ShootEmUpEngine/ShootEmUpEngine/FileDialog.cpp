#include "FileDialog.h"



FileDialog::FileDialog()
{

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
		for (int i = 0; i < fileName.size(); i++){saveFile[i] = fileName[i];}

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

string FileDialog::GetFile()
{
	char file[MAX_PATH];
	OPENFILENAME fileDialog;

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
