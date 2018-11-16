#ifndef LABEL_H
#define LABEL_H

#include "Font.h"
#include "..\camera\Camera.h"
#include "..\math\Vector2.h"

class Label
{
private:
	Vector2 _Position;
	Vector2 _Size;
	LPCWSTR _Text;
	std::wstring stemp;
	Font *_Font;

public:
	Label();
	~Label();

	Label(const Label& label);
	Label& operator=(const Label &label);

	void SetText(std::string &text);
	Label(const std::string &text, Font *font, float x, float y, float width, float height);

	void Draw(Camera *cam);

	void SetPosition(float x, float y);
	void SetSize(float width, float height);
	void SetFont(Font *font);

};

#endif
