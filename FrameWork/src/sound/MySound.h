#pragma once
#include <iostream>
using namespace std;
#include "..\src\sound\DirectSound.h"

class Sound
{
private:
	static CSoundManager *dsound;

	Sound()  // Disallow creating an instance of this object
	{

	}

	~Sound()
	{

	}

public :
	static bool DirectSound_Init(HWND hwnd);
	static void DirectSound_Shutdown();
	static CSound *LoadSound(string filename);
	static void Play(CSound *sound);
	static void Loop(CSound *sound);
	static void Stop(CSound *sound);

};

