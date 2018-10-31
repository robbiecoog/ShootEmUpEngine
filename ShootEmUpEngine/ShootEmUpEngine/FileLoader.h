#pragma once
#include <iostream>
#include <fstream>
#include "SDL_rwops.h"


using namespace std;

class FileLoader
{
public:

	//will be the data we are saving and loading
	Sint32 data[20];
	//file we are opening file for reading in binary
	SDL_RWops* file;

	bool fileOpen;

	FileLoader();
	~FileLoader();

};

