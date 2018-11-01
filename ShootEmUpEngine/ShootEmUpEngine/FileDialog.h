#pragma once
#include<iostream>
#ifndef _FILE_DIALOG_H_
#define _FILE_DIALOG_H_
#include <Windows.h>

using namespace::std;
class FileDialog
{
public:
	
	FileDialog();
	 void GetOpenFile();
	 void SaveFile();
	 void CheckErrors();
	 ~FileDialog();
};
#endif
