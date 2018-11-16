#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "..\math\Vector2.h"

//
class Camera:public GameObject
{
private:
	Vector2 _Position; //The camera position
	//Vector2 _Size; //the size of this camera
	//Vector2 _ScaleFactor; 

public:
	
	Camera();
	Camera(float x, float y, float sizeX, float sizeY);
	~Camera();

	const Vector2& GetPosition() const;
	//Vector2 GetSize();
	//Vector2 GetScaleFactor();

	//Set position
	void SetPosition(float x, float y);

	////Not available currently
	////Note that if the size of screen is different from the size of camera, consider using resize method
	//void SetSize(float x, float y);

	////Resize 
	//void Resize(float x, float y);


};

#endif