#include "Math2D.h"

const Vector2& Math2D::Lerp(const Vector2& vectorA, const Vector2& vectorB, float t)
{
	Vector2 result = Vector2(vectorA.x + t * (vectorB.x - vectorA.x), vectorA.y + t * (vectorB.y - vectorA.y));

	return result;
}