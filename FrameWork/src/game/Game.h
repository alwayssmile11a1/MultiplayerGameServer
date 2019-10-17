#ifndef GAME_H
#define GAME_H

#include "..\multiplayercore\NetworkManager.h"
#include <time.h>
#include <Windows.h>
#include "GameObject.h"
#include "..\others\Global.h"
#include "..\scene\Scene.h"
#include "..\sound\MySound.h"
#include "..\input\Input.h"
#include "..\others\Utility.h"
#include "..\console\Debug.h"
#include "..\math\Time.h"

	//this abstract class manages game structure, such as DirectX, game run, etc... 
class Game : public GameObject
{
private:

	//Window related variables
	HINSTANCE _HInstance; //handle for a window instance
	HWND _HWnd;				// Handle of the Game Window
	LPWSTR _WindowName; //the application title
	bool _IsFullScreen; //fullscreen or not?
	int _FrameRate; //how many frames in one second?

	//Direct3D variables
	LPDIRECT3DSURFACE9 _Backbuffer;
	LPDIRECT3DSURFACE9 _Background;
	D3DCOLOR _BackGroundColor;
	LPWSTR _BackGroundFilePath;

	//window event callback function - basically we use this function to determine what message is sent to Window
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//time between two frames
	DWORD DeltaTime;

	//store the client rect
	//RECT _WindowRect;

	//init
	bool InitWindow();
	bool InitDirectX();
	bool InitGame();


	//Scene
	Scene* _CurrentScene;

protected:
	//RECOMMEND: overide these two function and do all the things you want
	//create all the things of a game here, such as world, object, ...
	void virtual CreateGame() = 0;
	//update game by one frame (render, control game) - just write your game here
	void virtual UpdateGame(float dt) = 0;

	//void virtual Resize(float x, float y);

	//set background color. If you have already set background image, this function has no affect at all
	void SetBackGroundColor(D3DCOLOR color);
	//create a background from this image. If you want to disable this function, just set the filePath to be NULL
	void SetBackGroundImage(LPWSTR filePath);

	void SetScene(Scene *scene);

	//call postmessage to shut down application
	void ShutDownApplication();

public:
	//constructor and destructor
	Game();
	Game(HINSTANCE hInstance, LPWSTR windowName, int screenWidth, int screenHeight, bool isFullScreen, int frameRate);
	virtual ~Game();

	//void SetInstance(HINSTANCE hInstance);

	//RECOMMEND: don't modify this function, just call it in the main function
	//do things in the updateGame function
	//and this function will take care the rest for you
	int RunGame();

	void ShowGUILog();

	int GetFrameRate() { return _FrameRate; };

	//Release game
	void virtual Release() = 0;

};

#endif

