#ifndef TMXOBJECTGROUP_H
#define TMXOBJECTGROUP_H

#include <string>
#include <vector>
#include "..\shape\Rectangle.h"


class TMXObjectGroup
{
private:
	std::string _Name;
	std::vector<Shape::Rectangle> _Rectangles;


	void SetAttributes(const std::string& name);
	void AddRect(const Shape::Rectangle &rectangle);


	friend class TMXLoader;

public:
	TMXObjectGroup();
	~TMXObjectGroup();
	//TMXObjectGroup(const TMXObjectGroup &tmxObjectGroup);
	//TMXObjectGroup& operator=(const TMXObjectGroup &tmxObjectGroup);


	std::string GetName();
	const std::vector<Shape::Rectangle>& GetRects() const;

};

#endif