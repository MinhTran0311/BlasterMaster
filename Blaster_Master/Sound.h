#pragma once
#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include "global.h"
#include "Game.h"
#include <dsound.h>
#include <d3dx9.h>
#include <d3d9.h>
#include "tinyxml.h"

#define SOUND_RESOURCE_INTRO "Resource/Sound/SoundIntro.xml"
#define SOUND_RESOURCE_UNDERWORLD "Resource/Sound/SoundPlayScene.xml"
#define SOUND_RESOURCE_ENDING "Resource/Sound/SoundEnding.xml"
#define SOUND_RESOURCE_CHOOSING_WEAPON "Resource/Sound/ChooseWeapon.xml"

class Sound
{
private:
	Sound(HWND hWnd);
	static Sound* Instance;
	IDirectSoundBuffer* primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*> soundBufferMap;
	IDirectSound8* pDevice;
	bool isMute;
	float volume;
	std::map<std::string, IDirectSoundBuffer8*> soundTemp; // sound lưu thông tin để resume

public:
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	static Sound* GetInstance();
	void static Create(HWND hWnd);
	void Play(std::string name, bool infiniteLoop, int times = 100000);
	void Stop(std::string name = "");
	void LoadSound(std::string fileName, std::string name);
	void UnLoadSound(std::string name);
	void LoadSoundResource(const char*);
	void StopAll();
	~Sound();
};



