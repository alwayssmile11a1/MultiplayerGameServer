#include "Label.h"



Label::Label()
{
	_Font = NULL;
}


Label::~Label()
{
	
}


Label::Label(const std::string &text, Font* font, float x, float y, float width, float height)
{
	_Font = font;
	_Position.Set(x, y);
	_Size.Set(width, height);


	//get LPCWSTR from text
	stemp = StringToWstring(text);
	_Text = stemp.c_str();
}


Label::Label(const Label& label)
{
	_Position = label._Position;
	_Size = label._Size;
	_Font = label._Font;
	stemp = label.stemp;
	_Text = stemp.c_str();
	//std::vector<wchar_t> thecopy(wcslen(label._Text) + 1); // add one for null terminator
	//wcscpy_s(thecopy.data(), thecopy.size(), label._Text);

	//// you can get a pointer to the copy this way:
	//_Text = thecopy.data();



}
Label& Label::operator=(const Label &label)
{
	_Position = label._Position;
	_Size = label._Size;
	_Font = label._Font;
	stemp = label.stemp;
	_Text = stemp.c_str();

	//std::vector<wchar_t> thecopy(wcslen(label._Text) + 1); // add one for null terminator
	//wcscpy_s(thecopy.data(), thecopy.size(), label._Text);

	// you can get a pointer to the copy this way:
	//_Text = thecopy.data();

	return *this;
}

void Label::Draw(Camera *cam)
{
	if (_Font == NULL || cam == NULL) return;

	D3DXVECTOR3 postion;
	postion.x = _Position.x;
	postion.y = _Position.y;
	postion.z = 0;

	//get actual position
	D3DXMATRIX _CameraMatrix;
	D3DXVECTOR4 _ActualPosition; //the actual position of the sprite will be drawn
								 //get the actual postion
	D3DXMatrixIdentity(&_CameraMatrix);
	_CameraMatrix._22 = -1;
	_CameraMatrix._41 = -(cam->GetPosition().x - screenWidth / 2.0);
	_CameraMatrix._42 = +cam->GetPosition().y + screenHeight / 2.0;

	D3DXVec3Transform(&_ActualPosition, &postion, &_CameraMatrix);
	postion.x = _ActualPosition.x;
	postion.y = _ActualPosition.y;
	postion.z = 0;

	RECT rect = { postion.x,postion.y,_Size.x,_Size.y };

	
	////get LPCWSTR from _Text
	//std::wstring stemp = StringToWstring(_Text);
	//LPCWSTR text = stemp.c_str();

	_Font->_pFont->DrawText(NULL, _Text, -1, &rect, DT_TOP | DT_LEFT,
		D3DCOLOR_XRGB(255, 255, 255));

}

void Label::SetText(std::string &text)
{
	//get LPCWSTR from text
	 stemp = StringToWstring(text);
	_Text = stemp.c_str();
}

void Label::SetPosition(float x, float y)
{
	_Position.Set(x, y);
}
void Label::SetSize(float width, float height)
{
	_Size.Set(width, height);
}

void Label::SetFont(Font *font)
{
	_Font = font;
}