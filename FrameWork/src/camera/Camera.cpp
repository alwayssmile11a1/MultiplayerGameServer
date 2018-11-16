
#include"Camera.h"

Camera::Camera()
{
	_Position.Set(0, 0);
	//_Size.Set(0, 0);
	//_ScaleFactor.Set(1, 1);
}
Camera::Camera(float x, float y, float sizeX, float sizeY)
{
	_Position.Set(x, y);
	//_Size.Set(sizeX, sizeY);
	//_ScaleFactor.Set(1, 1);
}

Camera::~Camera()
{

}

const Vector2& Camera::GetPosition() const
{
	return _Position;
}

void Camera::SetPosition(float x, float y)
{
	_Position.Set(x, y);
}

//void Camera::SetSize(float x, float y)
//{
//	_Size.Set(x, y);
//}
//
//Vector2 Camera::GetSize()
//{
//	return _Size;
//}

//void Camera::Resize(float x, float y)
//{
//	_ScaleFactor.Set(_Size.x / x, _Size.y / y);
//}

//Vector2 Camera::GetScaleFactor()
//{
//	return _ScaleFactor;
//}