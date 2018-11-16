#ifndef BODYDEF_H
#define BODYDEF_H

#include "..\math\Vector2.h"
#include "Body.h"

class BodyDef
{
public:
	Vector2 position;
	Vector2 size;
	float mass;
	Body::BodyType bodyType;
	bool isSensor;
	Vector2 linearDrag;


public:
	BodyDef();
	~BodyDef();
	BodyDef(float posx, float posy, float sizex, float sizey, Body::BodyType bodyType, bool isSensor);
};

#endif
