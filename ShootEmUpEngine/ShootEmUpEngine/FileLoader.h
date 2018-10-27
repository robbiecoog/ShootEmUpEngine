#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class FileLoader
{
public:
	string state;
	string textSave;
	ifstream file;
	char* buffer;
	long fileSize;

	FileLoader();
	~FileLoader();
	void LoadFile();
	void SaveFile();

};

