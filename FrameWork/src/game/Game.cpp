#include"Game.h"



Game::Game()
{

}

Game::Game(HINSTANCE hInstance, LPWSTR windowName, int _screenWidth, int _screenHeight, bool isFullScreen, int frameRate)
{
	this->_HInstance = hInstance;
	this->_WindowName = windowName;
	this->_IsFullScreen = isFullScreen;
	this->_FrameRate = frameRate;
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	_BackGroundColor = D3DCOLOR_XRGB(0, 0, 0);
	_BackGroundFilePath = NULL;
	_HWnd = NULL;
	_Backbuffer = NULL;
	_Background = NULL;
}

//void Game::SetInstance(HINSTANCE hInstance)
//{
//	this->_HInstance = hInstance;
//}

Game::~Game()
{
	////free the backbuffer
	//if (_Backbuffer != NULL)
	//{
	//	_Backbuffer->Release();
	//	_Backbuffer = NULL;
	//}

	////free the _Background
	//if (_Background != NULL)
	//{
	//	_Background->Release();
	//	_Background = NULL;
	//}

	////Release DirectXDevice
	////release the Direct3D device
	//if (d3ddevice != NULL) {
	//	d3ddevice->Release();
	//	d3ddevice = NULL;
	//}

	////release the Direct3D object
	//if (d3d != NULL)
	//{
	//	d3d->Release();
	//	d3d = NULL;
	//}

	////Release input
	//input.Release();

}


//Release game
void Game::Release()
{
	//free the backbuffer
	if (_Backbuffer != NULL)
	{
		_Backbuffer->Release();
		_Backbuffer = NULL;
	}

	//free the _Background
	if (_Background != NULL)
	{
		_Background->Release();
		_Background = NULL;
	}

	//Release DirectXDevice
	//release the Direct3D device
	if (d3ddevice != NULL) {
		d3ddevice->Release();
		d3ddevice = NULL;
	}

	//release the Direct3D object
	if (d3d != NULL)
	{
		d3d->Release();
		d3d = NULL;
	}

	//Release input
	Input::Release();

	//shut down for sure
	Sound::DirectSound_Shutdown();

	//Clean networking stuff
	NetworkManager::CleanUp();
}

void Game::ShowGUILog()
{
#ifdef _DEBUG
	RedirectIOToConsole();
#endif
}

void Game::SetBackGroundColor(D3DCOLOR color)
{
	_BackGroundColor = color;
}

void Game::SetBackGroundImage(LPWSTR filePath)
{
	_BackGroundFilePath = filePath;

	//Release first for sure
	_Background->Release();
	_Background = NULL;

	if (_BackGroundFilePath == NULL)
	{
		// Create background
		d3ddevice->CreateOffscreenPlainSurface(
			100,					// width 				
			100,					// height
			D3DFMT_X8R8G8B8,		// format
			D3DPOOL_DEFAULT,		// where? (VRAM or RAM)
			&_Background,
			NULL);
	}
	else
	{
		_Background = CreateSurfaceFromFile(d3ddevice, _BackGroundFilePath);
	}
}

LRESULT CALLBACK Game::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool Game::InitWindow()
{
	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//fill in the struct paramaters
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _HInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _WindowName;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	//Window style
	DWORD WINDOWSTYLE;
	if (_IsFullScreen)
	{
		WINDOWSTYLE = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP; //should use this for full screen mode
	}
	else
	{
		WINDOWSTYLE = WS_OVERLAPPEDWINDOW; //should use this for window mode
	}

	//create the window
	_HWnd = CreateWindowEx(NULL,
		_WindowName,	//nam of window class
		_WindowName,	//title bar
		WINDOWSTYLE,	//window style
		CW_USEDEFAULT, //x position of window
		CW_USEDEFAULT, //y position of window
		screenWidth, //width of the window
		screenHeight, //height of the window,
		NULL, //parent window = none
		NULL, //menu = none
		_HInstance, //application Instance
		NULL //window parameters
	);

	//if something bad happened, return
	if (!_HWnd)
		return false;

	//display the window
	ShowWindow(_HWnd, SW_SHOWNORMAL);
	UpdateWindow(_HWnd);

	return true;
}


bool Game::InitDirectX()
{
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL) {
		MessageBox(_HWnd, L"Error initializing Direct3D", L"Error", MB_OK);
		return false;
	}

	//set DIRECT3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	if (_IsFullScreen)
	{
		d3dpp.Windowed = false; //full screen or not?
	}
	else
	{
		d3dpp.Windowed = true;
	}
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.hDeviceWindow = _HWnd;

	//create Direct3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		_HWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddevice
	);

	if (d3ddevice == NULL) {
		MessageBox(_HWnd, L"Error creating Direct3D device", L"Error", MB_OK);
		return false;
	}


	//set random number seed
	srand(time(NULL));

	//clear the backbuffer to black
	d3ddevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);


	// Create background
	d3ddevice->CreateOffscreenPlainSurface(
		100,					// width 				
		100,					// height
		D3DFMT_X8R8G8B8,		// format
		D3DPOOL_DEFAULT,		// where? (VRAM or RAM)
		&_Background,
		NULL);


	//return okay
	return true;
}

bool Game::InitGame()
{
	//init window
	if (!InitWindow())
	{
		return false;
	}
	
	//init directX
	if (!InitDirectX())
	{
		return false;
	}

	//init input manager
	Input::Init(_HInstance, _HWnd);

	//Init sound
	Sound::DirectSound_Init(_HWnd);

	//Init networking 
	NetworkManager::StaticInit();

	//create game
	CreateGame();

	return true;
}

int Game::RunGame()
{

	if (!InitGame()) return 0;

	//message from window
	MSG msg;

	//a variable to let us know the starting time of a frame
	DWORD frame_start = GetTickCount();
	//the average time per frame
	DWORD count_per_frame = 1000 / _FrameRate;

	//game loop
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			//Translate message and dispatch to WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			//wanna quit?
			if (msg.message == WM_QUIT)
				break;
		}
		
		Input::ProcessKeyBoardInformation();
		Input::ProcessMouseInformation();

		//get "now" time
		DWORD now = GetTickCount();
		//calculate delta time
		DeltaTime = now - frame_start;
		if (DeltaTime >= count_per_frame) //if true, next frame
		{
			frame_start = now;

			//get rectsize
			//GetWindowRect(_HWnd, &_WindowRect);
			//Resize(_WindowRect.right - _WindowRect.left, _WindowRect.bottom - _WindowRect.top);

			//make sure the Direct3D device is valid
			if (d3ddevice == NULL)
			{
				return 0;
			}

			//start rendering
			if (d3ddevice->BeginScene())
			{
				//Clear back ground with a colored surface
				if (_BackGroundFilePath == NULL)
				{
					// Fill the bitmap
					d3ddevice->ColorFill(_Background, NULL, _BackGroundColor);
					
					// Draw the surface onto the back buffer
					d3ddevice->StretchRect(
									_Background,			// from 
									NULL,				// which portion?
									_Backbuffer,		// to 
									NULL,				// which portion?
									D3DTEXF_NONE);
				}
				else //Clear back ground with an image
				{
					
					//draw surface to backbuffer
					d3ddevice->StretchRect(_Background, NULL, _Backbuffer, NULL, D3DTEXF_NONE);
				}

				//
				UpdateGame(DeltaTime/1000.0f);

				//clear buffedInput after we has done update game
				Input::ClearBuffedInput();

				//create pointer to the backbuffer
				d3ddevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_Backbuffer);

				//stop rendering
				d3ddevice->EndScene();
			}

			//Display the backbuffer on the screen
			d3ddevice->Present(NULL, NULL, NULL, NULL);

		}
		else
		{
			//sleep
			Sleep(count_per_frame - (now - frame_start));
		}

	}

	return msg.wParam;

}

void Game::UpdateGame(float dt)
{
	if (_CurrentScene != nullptr)
	{
		_CurrentScene->Update(dt);
	}
}

void Game::SetScene(Scene *scene)
{
	_CurrentScene = scene;
}

void Game::ShutDownApplication()
{
	//trace(L"Escape key pressed!");
	PostMessage(_HWnd, WM_QUIT, 0, 0);
}