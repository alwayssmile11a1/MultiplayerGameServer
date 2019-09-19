#ifndef MATH2D_H
#define MATH2D_H
#include "Vector2.h"

class Math2D
{
public:

	static const Vector2& Lerp(const Vector2& vectorA, const Vector2& vectorB, float t);
	
};

#endif