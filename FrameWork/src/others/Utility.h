//useful functions
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "..\extensions\directX9\Include\d3d9.h"
#include "..\extensions\directX9\Include\d3dx9.h" //please read the readme.txt file if you have any problems with this include line. 

LPDIRECT3DSURFACE9 CreateSurfaceFromFile(LPDIRECT3DDEVICE9 d3ddv, LPWSTR FilePath);

/* File loading helper functions */
bool LoadFile(std::string filePath, std::string &fileContents);

std::wstring StringToWstring(const std::string& s);