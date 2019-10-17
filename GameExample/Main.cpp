#pragma once
#include "GameExample.h"

// application title
//(since we use Unicode character set, //we have to put L before "GameExample". 
//This means the "GameExample" should be made of 16 bit Unicode characters, rather than 8 bit)
#define APPTITLE L"GameExample"

//Full screen or not
#define FULLSCREENMODE FALSE

//Screen resolution
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

//frame rate 
#define FRAME_RATE 60

//Game* GetApplication() 
//{
//	wstring title = APPTITLE;
//	GameExample* game = new GameExample(&title[0], SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREENMODE, FRAME_RATE);
//}

//The entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	wstring title = APPTITLE;
	GameExample game(hInstance, &title[0], SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREENMODE, FRAME_RATE);
	game.ShowGUILog();
	game.RunGame();
	game.Release();
}