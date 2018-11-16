#include "BodyDef.h"

BodyDef::BodyDef()
{
	position.Set(0, 0);
	size.Set(0, 0);
	mass = 1;
	bodyType = Body::BodyType::Static;
	isSensor = false;
	linearDrag.Set(1, 1);
}


BodyDef::~BodyDef()
{
}
