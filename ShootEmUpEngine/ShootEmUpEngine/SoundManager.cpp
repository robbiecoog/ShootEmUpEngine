#include "SoundManager.h"


Mix_Music* SoundManager::GetMusic(string filename)
{
	string filePath = SDL_GetBasePath();
	filePath.append("Assets/" + filename);

	if (_music[filePath] == nullptr)
	{
		_music[filePath] = Mix_LoadMUS(filePath.c_str());

		if (_music[filePath] = NULL)
		{
			printf("Music Loading Error: ", filename.c_str(), Mix_GetError());
		}
		return _music[filePath];
	}
}

Mix_Chunk* SoundManager::GetSFX(string filename)
{
	string filePath = SDL_GetBasePath();
	filePath.append("Assets/" + filename);

	if (_SFX[filePath] == nullptr)
	{
		_SFX[filePath] = Mix_LoadWAV(filePath.c_str());

		if (_SFX[filePath] == NULL)
		{
			printf("SFX Loading Error: ", filename.c_str(), Mix_GetError());
		}
		return _SFX[filePath];
	}
}

SoundManager* SoundManager::instance = NULL;

SoundManager* SoundManager::getInstance() 
{
	if (instance == NULL)
	{
		instance == new SoundManager();
	}
	return instance;
}

SoundManager::SoundManager()
{
	getInstance();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		printf("Mixer Initialization Error: ", Mix_GetError());
	}
}


SoundManager::~SoundManager()
{
	delete instance;
	instance = NULL;
	Mix_Quit();

}

void SoundManager::Update(SDL_Event* e)
{
	PlayMusic("background");
	//PlaySFX("");
}

void SoundManager::PlayMusic(string filename, int loops)
{
	Mix_PlayMusic(GetMusic(filename), loops);
}

void SoundManager::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
	}
}

void SoundManager::ResumeMusic()
{
	if (Mix_PausedMusic() != 0)
	{
		Mix_ResumeMusic();

	}
}

void SoundManager::PlaySFX(string filename, int loops, int channel)
{
	Mix_PlayChannel(channel, GetSFX(filename), loops);
}