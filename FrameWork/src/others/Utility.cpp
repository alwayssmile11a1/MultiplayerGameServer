#include "Utility.h"


LPDIRECT3DSURFACE9 CreateSurfaceFromFile(LPDIRECT3DDEVICE9 d3ddv, LPWSTR FilePath)
{
	D3DXIMAGE_INFO info;

	HRESULT result = D3DXGetImageInfoFromFile(FilePath, &info);
	if (result != D3D_OK)
	{
		return NULL;
	}
	
	LPDIRECT3DSURFACE9 surface = NULL;

	d3ddv->CreateOffscreenPlainSurface(
		info.Width,				// width
		info.Height,			// height
		D3DFMT_X8R8G8B8,		// format
		D3DPOOL_DEFAULT,
		&surface,
		NULL);

	result = D3DXLoadSurfaceFromFile(
		surface, 		// surface
		NULL,			// destination palette	
		NULL,			// destination rectangle 
		FilePath,
		NULL,			// source rectangle
		D3DX_DEFAULT, 	// filter image
		D3DCOLOR_XRGB(0, 0, 0),				// transparency (0 = none)
		NULL);			// reserved

	if (result != D3D_OK)
	{
		return NULL;
	}

	return surface;
}

bool LoadFile(std::string filePath, std::string &fileContents)
{
	std::ifstream file(filePath, std::ios::in | std::ios::binary);

	if (file)
	{
		// move the file pointer to the end of the file
		file.seekg(0, std::ios::end);

		//file.tellg is used to get the position of the file pointer, which also means the size of the file in this case
		fileContents.resize(file.tellg());

		//move the file pointer to the beginning of the file
		file.seekg(0, std::ios::beg);

		//read this file into string
		file.read(&fileContents[0], fileContents.size());
		file.close();

		return true;
	}
	return false;
}

std::wstring StringToWstring(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	return r;
}