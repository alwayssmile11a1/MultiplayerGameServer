#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "GameObject.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "Sprite.h"
#include "..\camera\Camera.h"
#include "..\others\Global.h"

#define Pi 3.14159265

struct CUSTOMVERTEX
{
	float x, y, z, weight;
	DWORD color;
};


//Manage drawing things
//Drawing must be called between the begin and end methods
//RECOMMEND: Consider using only one sprite batch throughout whole game
//And SpriteBatch is not automatically released, you have to call the release method yourself
class SpriteBatch: public GameObject
{
private:
	LPD3DXSPRITE _SpriteHandler; //handle to a sprite 
	
	D3DXMATRIX _SpriteMatrix; // Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX _CameraMatrix; // Build our matrix to rotate, scale and position our sprite
	D3DXVECTOR4 _ActualPosition; //the actual position of the sprite will be drawn
	RECT _Rect; //the portion of image we want to draw
	D3DXVECTOR3 _Center; 
	D3DXVECTOR3 _Position; //position to draw in our world
	D3DXVECTOR2 _RectSize; //the sprite bounds
	D3DXVECTOR2 _ScaleFactor; //scale
	D3DXVECTOR2 _RotationOrigin; //the position that will be used as origin for rotating
	D3DXVECTOR2 _ScaleOrigin; //the original position of a sprite used for scaling
	float _RotationFactor; //rotation

	Camera *_Camera; //the camera is being used by this batch


	void GetActualPosition(D3DXVECTOR3 * postion);

	bool IsOutsideCam(float x, float y, float width, float height);

public:
	SpriteBatch();
	~SpriteBatch();

	//Create batch
	void Create();

	//Release this batch
	void Release();

	//Set the camera to be used by this batch
	void SetCamera(Camera *camera);

	//Draw texture at (x,y)
	void Draw(const Texture &texture, float x, float y);

	//Draw textureregion at (x,y)
	void Draw(const TextureRegion &textureRegion, float x, float y);

	//draw the texture at (x,y) and stretch it to width and height
	void Draw(const Texture &texture, float x, float y, float width, float height);

	//draw or portion of texture at (x,y) and stretch it to width and height
	void Draw(const Texture &texture, float x, float y, float rectLeft, float rectTop, float rectWidth, float rectHeight, float width, float height);

	void Draw(const Sprite &sprite);

	void DrawSquare(float x, float y, float width, float height, D3DCOLOR color);

	//begin drawing
	void Begin();

	//end drawing
	void End();

};

#endif