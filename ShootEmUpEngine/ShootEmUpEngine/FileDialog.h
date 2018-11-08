#pragma once
#include<iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#ifndef _FILE_DIALOG_H_
#define _FILE_DIALOG_H_


using namespace::std;

class FileDialog
{
public:
	
	FileDialog();
	~FileDialog();
	 void GetOpenFile();
	 void SaveFile();
	 void CheckErrors();
};
#endif
