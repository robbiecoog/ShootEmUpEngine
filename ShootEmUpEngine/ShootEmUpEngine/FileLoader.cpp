#include "FileLoader.h"



FileLoader::FileLoader()
{
	/* opening the save file for reading from the SDL_RWFromFile function
	first argument is file path we are opening from and the second argument is how we are opening it */
	file = SDL_RWFromFile("C:_Users_j015791e_Documents_Github_ShootEmUpEngine_ShootEmUpEngine_x64_Debug", "r+b");

	//file doesn't exist - will open a new file for writing in binary
	if (file == NULL)
	{
		printf("Warning: unable to open File! SDL Error: %s\n", SDL_GetError());

		//create file to write
		file = SDL_RWFromFile("C:_Users_j015791e_Documents_Github_ShootEmUpEngine_ShootEmUpEngine_x64_Debug", "w+b");

		if (file != NULL)
		{
			printf("New File Created\n");

			//Initialising the data
			for (int i = 0; i < 20; ++i)
			{
				data[i] = 0;
				SDL_RWwrite(file, &data[i], sizeof(Sint32), 1);
			}
			SDL_RWclose(file);
		}
		else
		{
			printf("Error: unable to create file! SDL Error:%s\n", SDL_GetError());
			fileOpen = false;
		}
	}
	//file exists
	else 
	{
		//load the data
		printf("Reading File...!\n");

		for (int i = 0; i < 20; ++i)
		{
			SDL_RWread(file, &data[i], sizeof(Sint32), 1);
		}
		SDL_RWclose(file);
	}

	//initialise data textures

}


FileLoader::~FileLoader()
{
	
}
