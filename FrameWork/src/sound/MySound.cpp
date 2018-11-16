#include "MySound.h"

CSoundManager* Sound::dsound = NULL;

// New DirectSound code
//primary DirectSound object
 bool Sound:: DirectSound_Init(HWND hwnd)
{
	//create DirectSound manager object
	dsound = new CSoundManager();
	//initialize DirectSound
	HRESULT result;
	result = dsound->Initialize(hwnd, DSSCL_PRIORITY);
	if (result != DS_OK) return false;
	//set the primary buffer format
	result = dsound->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK) return false;
	//return success
	return true;
}
void Sound::DirectSound_Shutdown()
{
	if (dsound) delete dsound;
}
CSound  *Sound::LoadSound(string filename)
{
	HRESULT result;
	//create local reference to wave data
	CSound *wave = NULL;
	////attempt to load the wave file
	//char s[255];
	//sprintf(s,"%s", filename.c_str());//-----------------------------

				
	int len;
	int slength = (int)filename.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, filename.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, filename.c_str(), slength, buf, len);

	result = dsound->Create(&wave,buf);//------------------------------------
	if (result != DS_OK) wave = NULL;

	delete[] buf;

	//return the wave
	return wave;
}

void Sound::Play(CSound *sound)
{
	sound->Play();
}
void Sound::Loop(CSound *sound)
{
	sound->Play(0, DSBPLAY_LOOPING);
}
void Sound::Stop(CSound *sound)
{
	sound->Stop();
}