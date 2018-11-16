#ifndef FONT_H
#define FONT_H

#include "..\extensions\directX9\Include\d3dx9.h"
#include "..\others\Global.h"
#include <string>
#include "..\src\others\Utility.h"

class Font
{
private:
	ID3DXFont* _pFont;

private:
	friend class Label;

public:
	Font();
	~Font();

	Font(const std::string &fontName, int width, int height);
	
	void Release();
};

#endif

