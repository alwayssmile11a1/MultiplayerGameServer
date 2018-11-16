#include "Font.h"

Font::Font()
{
	_pFont = NULL;
}
Font::~Font()
{
	
}

Font::Font(const std::string &fontName,int width,int height)
{
	//get path from string
	std::wstring stemp = StringToWstring(fontName);
	LPCWSTR font = stemp.c_str();

	HRESULT result = D3DXCreateFontW(d3ddevice, height, width, 1, FW_NORMAL, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, font, &_pFont);

	if (!SUCCEEDED(result))
	{
		//failed
	}

	

}

void Font::Release()
{
	_pFont->Release();
}

