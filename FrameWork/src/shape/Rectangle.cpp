#include "Rectangle.h"

namespace Shape
{
	Rectangle::Rectangle()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}


	Rectangle::~Rectangle()
	{
	}

	Rectangle::Rectangle(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void Rectangle::Set(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
}