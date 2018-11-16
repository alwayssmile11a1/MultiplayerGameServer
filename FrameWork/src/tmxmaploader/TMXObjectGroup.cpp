#include "TMXObjectGroup.h"



TMXObjectGroup::TMXObjectGroup()
{
	_Rectangles.clear();
}


TMXObjectGroup::~TMXObjectGroup()
{
	/*for (std::vector<Shape::Rectangle>::iterator rect = _Rectangles.begin(); rect != _Rectangles.end(); ++rect)
	{
		delete *rect;
	}*/
}

//TMXObjectGroup::TMXObjectGroup(const TMXObjectGroup &tmxObjectGroup)
//{
//	_Name = tmxObjectGroup._Name;
//	for ( std::vector<_Rectangles*>:: const_iterator bo = tmxObjectGroup._Bodies.begin(); bo != tmxObjectGroup._Bodies.end(); ++bo)
//	{
//		Body*body = new Body();
//		*body = **bo;
//
//		_Bodies.push_back(body);
//	}
//
//}
//TMXObjectGroup& TMXObjectGroup::operator=(const TMXObjectGroup &tmxObjectGroup)
//{
//	_Name = tmxObjectGroup._Name;
//	for (std::vector<Body*>::const_iterator bo = tmxObjectGroup._Bodies.begin(); bo != tmxObjectGroup._Bodies.end(); ++bo)
//	{
//		Body*body = new Body();
//		*body = **bo;
//
//		_Bodies.push_back(body);
//	}
//
//	return *this;
//}

void TMXObjectGroup::SetAttributes(const std::string& name)
{
	_Name = name;
}
void TMXObjectGroup::AddRect(const Shape::Rectangle &rect)
{
	_Rectangles.push_back(rect);
}

std::string  TMXObjectGroup::GetName()
{
	return _Name;
}

const std::vector<Shape::Rectangle>& TMXObjectGroup::GetRects() const
{
	return _Rectangles;
}