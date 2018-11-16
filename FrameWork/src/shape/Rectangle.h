#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace Shape
{

	class Rectangle
	{
	public:
		int id; //
		float extraX, extraY;

		float x, y, width, height;

	public:
		Rectangle();
		~Rectangle();
		Rectangle(float x, float y, float width, float height);
		
		void Set(float x, float y, float width, float height);

	};

}

#endif

