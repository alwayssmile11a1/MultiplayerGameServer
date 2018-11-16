#ifndef SPACEDIVISIONQUADTREE_H
#define SPACEDIVISIONQUADTREE_H

#include "..\others\Utility.h"
#include <map>
#include <vector>
#include "..\world\Body.h"
#include "..\extensions\rapidxml\rapidxml.hpp"
#include "..\extensions\rapidxml\rapidxml_print.hpp"
#include "..\world\Collision.h"
#include "Windows.h"
#include "..\camera\Camera.h"
#include "..\others\Global.h"
#include "..\shape\Rectangle.h"
//#include "..\world\World.h"
#include "..\tmxmaploader\TMXTileLayer.h"


class SDQObject
{
private:
	bool isContainBody; //if true, contain body. if false, contain Rectangle
	unsigned int id;
	RECT rect;

	Body* body;
	Shape::Rectangle* tileRectangle;

	friend class SpaceDivisionQuadTree;

public:
	SDQObject()
	{
		body = NULL;
		tileRectangle = NULL;
	}

	SDQObject(unsigned int id, Body* body,Shape::Rectangle* tileRectangle)
	{
		this->id = id;
		this->body = body;
		this->tileRectangle = tileRectangle;

	}

	void Release()
	{
		if (tileRectangle != NULL)
		{
			delete tileRectangle;
			tileRectangle = NULL;
		}
	}
};

class SDQNode
{
private:
	RECT rect;
	unsigned int parentID;
	unsigned int id;
	SDQNode *tl, *tr, *bl, *br;
	std::vector<SDQObject> sdqObjects;
	
	friend class SpaceDivisionQuadTree;

public:

	SDQNode()
	{
		tl = nullptr;
		tr = nullptr;
		bl = nullptr;
		br = nullptr;
	}
	
	SDQNode(unsigned int id, unsigned int parentid)
	{
		tl = nullptr;
		tr = nullptr;
		bl = nullptr;
		br = nullptr;
		this->id = id;
		this->parentID = parentid;
	}

	void Release()
	{
		for (std::vector<SDQObject>::iterator it = sdqObjects.begin(); it != sdqObjects.end(); ++it)
		{
			it->Release();
		}
	}

	//unsigned int GetParentID()
	//{
	//	return parentID;
	//}

	//unsigned int GetID()
	//{
	//	return id;
	//}


};


//Performance issue somehow
//Used for TMX map to reduce the amount of tile have to draw
class SpaceDivisionQuadTree
{
private:
	//"write to xml" variables
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* rootXmlNode;
	std::string outpath;



	//"read from quadtree xml" variables
	std::map<unsigned int, Body*> mapBody;
	std::map<unsigned int, Shape::Rectangle*> mapTileRectangle;
	std::map<unsigned int, SDQNode*> mapQuadTree;
	SDQNode *rootSDQNode;

	//list all bodies within viewport
	std::vector<Body*> bodiesInViewport;
	//list all tile rectangle within the viewport
	std::vector<Shape::Rectangle*> tileRectsInViewport;

	float minSquareSize;

	bool loadBody;
	bool loadTileRect;

	World *world;

	std::map<std::string, std::vector<Body*>> bodyGroups;
	

private:

	friend class World;
	friend class TMXMap;


	void StartBuildingTreeNodes(const std::string &outpath, const std::string &tmxFilePath);
	//
	void BuildTree(SDQNode* sdqNode, rapidxml::xml_node<>* parentXmlNode);
	//
	void EndBuildingTreeNodes();
	//
	void Release(SDQNode* sdqNode);

	
	void Load(rapidxml::xml_node<>* xmlnode);

	void LoadObjectsInViewport(const RECT &viewport, SDQNode* sdqNode);

	void LinkNodes();

public:
	SpaceDivisionQuadTree();
	~SpaceDivisionQuadTree();

	void SetMinSquareSize(float minSquareXY);

	////if world is set, 
	//void SetWorld(World *world);

	//
	void BuildTreeNodesFromTMX(const std::string &outpath, const std::string &tmxFilePath);

	//Load tileRects and bodies from quadtreefile and tmxmapfile
	void Load(const std::string &quadtreeFilePath, const std::string &tmxFilePath);

	const std::map<unsigned int, SDQNode*>& GetMapQuadTree();

	const std::map<unsigned int, Body*>& GetMapBody();

	const std::map<unsigned int, Shape::Rectangle*>& GetMapTileRect();

	//SDQNode* GetRootSDQNode();

	void LoadObjectsInViewport(Camera *camera, bool loadBody, bool loadTileRect);


	const std::vector<Body*>& GetBodiesInViewport();

	const std::vector<Shape::Rectangle*>& GetTileRectsInViewport();


	const std::vector<Body*>& GetBodiesGroup(const std::string &groupName);

};


#endif 