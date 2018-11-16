#include "SpaceDivisionQuadTree.h"



SpaceDivisionQuadTree::SpaceDivisionQuadTree()
{
	minSquareSize = screenHeight;
	world = NULL;
}


SpaceDivisionQuadTree::~SpaceDivisionQuadTree()
{
	for (std::map<unsigned int, SDQNode*>::iterator it = mapQuadTree.begin(); it != mapQuadTree.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	for (std::map<unsigned int, Shape::Rectangle*>::iterator it = mapTileRectangle.begin(); it != mapTileRectangle.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	for (std::map<unsigned int, Body*>::iterator it = mapBody.begin(); it != mapBody.end(); ++it)
	{
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
}

void SpaceDivisionQuadTree::SetMinSquareSize(float minXY)
{
	minSquareSize = minXY;
}

void SpaceDivisionQuadTree::StartBuildingTreeNodes(const std::string &outpath, const std::string &tmxFilePath)
{
	this->outpath = outpath;

	//setup out xml file
	rapidxml::xml_node<>* declareNode = doc.allocate_node(rapidxml::node_declaration);
	declareNode->append_attribute(doc.allocate_attribute("version", "1.0"));
	declareNode->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(declareNode);

	rootXmlNode = doc.allocate_node(rapidxml::node_element, "spacedivisionmap");


	//load tmx file
	std::string fileContents = "";
	if (LoadFile(tmxFilePath, fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("map");

		int mapWidth = atoi(parentNode->first_attribute("width")->value());
		int mapHeight = atoi(parentNode->first_attribute("height")->value());
		int tileWidth = atoi(parentNode->first_attribute("tilewidth")->value());
		int tileHeight = atoi(parentNode->first_attribute("tileheight")->value());

		unsigned int sdmwidth = mapWidth*tileWidth;
		unsigned int sdmheight = mapHeight *tileHeight;
		char* value;
		if (sdmwidth > sdmheight)
		{
			//get value to append to the note
			value = doc.allocate_string(std::to_string(sdmwidth).c_str());
		}
		else
		{
			value = doc.allocate_string(std::to_string(sdmheight).c_str());
		}

		//appen width and height
		rootXmlNode->append_attribute(doc.allocate_attribute("width", value));
		rootXmlNode->append_attribute(doc.allocate_attribute("height", value));
		//append to xml doc 
		doc.append_node(rootXmlNode);


		//create a root SDQNode
		rootSDQNode = new SDQNode();
		rootSDQNode->parentID = 0;
		rootSDQNode->id = 0;
		//set rect
		rootSDQNode->rect.top = atoi(value);
		rootSDQNode->rect.left = 0;
		rootSDQNode->rect.bottom = 0;
		rootSDQNode->rect.right = atoi(value);

		//get all objects in objects group from tmx file and put to rootSDQNode
		rapidxml::xml_node<> * objectgroupNode = parentNode->first_node("objectgroup");

		while (objectgroupNode != nullptr)	
		{
			rapidxml::xml_node<> *currentTMXNode = objectgroupNode->first_node("object");

			while (currentTMXNode != nullptr)
			{
				unsigned int id = atoi(currentTMXNode->first_attribute("id")->value());
				unsigned int x = atoi(currentTMXNode->first_attribute("x")->value());
				unsigned int y = atoi(currentTMXNode->first_attribute("y")->value());
				unsigned int width = atoi(currentTMXNode->first_attribute("width")->value());
				unsigned int height = atoi(currentTMXNode->first_attribute("height")->value());

				SDQObject sdqObject;
				sdqObject.isContainBody = true;
				sdqObject.id = id;
				sdqObject.rect.top = sdmheight - y;
				sdqObject.rect.left = x;
				sdqObject.rect.bottom = sdmheight - y - height;
				sdqObject.rect.right = x + width;


				rootSDQNode->sdqObjects.push_back(sdqObject);

				currentTMXNode = currentTMXNode->next_sibling();
			}

			
			objectgroupNode = objectgroupNode->next_sibling();
		}

		//get tile data
		// Create a new node based on the parent node
		rapidxml::xml_node<> *layerNode = parentNode;
		// Move to first layer node
		layerNode = parentNode->first_node("layer");
		TMXTileLayer layer;
		if (layerNode != nullptr)
		{
			//get layer attributes
			unsigned int layerWidth = atoi(layerNode->first_attribute("width")->value());
			unsigned int layerHeight = atoi(layerNode->first_attribute("height")->value());

			//set layer attributes
			layer.SetAttributes("", layerWidth, layerHeight);

			//get layer data
			char* data = layerNode->first_node("data")->value();

			//set layer data
			layer.SetData(data);

		}

		//get tile set 
		// Create a new node based on the parent node
		rapidxml::xml_node<> *currentNode = parentNode;
		//get tileset node
		currentNode = currentNode->first_node("tileset");
		unsigned int tileSetWidth = 0;
		unsigned int tileSetHeight = 0;
		// Check if there is a tileset node
		if (currentNode != nullptr)
		{
			tileSetWidth = atoi(currentNode->first_attribute("tilewidth")->value());
			tileSetHeight = atoi(currentNode->first_attribute("tileheight")->value());
		}


		//load map tile rectangle
		unsigned int **data = layer.GetData();
		unsigned int layerWidth = layer.GetWidth();
		unsigned int layerHeight = layer.GetHeight();

		float x, y;
		float width = tileSetWidth;
		float height = tileSetHeight;
		unsigned int id = -1;
		for (unsigned int row = 0; row < layerHeight; row++)
		{

			for (unsigned int column = 0; column < layerWidth; column++)
			{
				id++;
				if (data[row][column] == 0) continue;

				x = column*width + width / 2;
				y = (layerHeight - 1 - row)*height + height / 2;
				
				SDQObject sdqObject;
				sdqObject.isContainBody = false;
				sdqObject.id = id;
				sdqObject.rect.top = y + height/2;
				sdqObject.rect.left = x - width/2;
				sdqObject.rect.bottom = y - height/2;
				sdqObject.rect.right = x + width/2;

				rootSDQNode->sdqObjects.push_back(sdqObject);


			}
		}

	}
}


void SpaceDivisionQuadTree::BuildTree(SDQNode* sdqNode, rapidxml::xml_node<>* parentXmlNode)
{
	if (sdqNode == nullptr) return;

	//allocate a sdq node
	rapidxml::xml_node<>* xmlNode = doc.allocate_node(rapidxml::node_element, "sdqnode");
	//append attribute id
	char* id = doc.allocate_string(std::to_string(sdqNode->id).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("id", id));
	//append attribute parent id
	char* parentid = doc.allocate_string(std::to_string(sdqNode->parentID).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("parentid", parentid));
	//append attribute rect
	char* rectTop = doc.allocate_string(std::to_string(sdqNode->rect.top).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("recttop", rectTop));
	char* rectLeft = doc.allocate_string(std::to_string(sdqNode->rect.left).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("rectleft", rectLeft));
	char* rectBottom = doc.allocate_string(std::to_string(sdqNode->rect.bottom).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("rectbottom", rectBottom));
	char* rectRight = doc.allocate_string(std::to_string(sdqNode->rect.right).c_str());
	xmlNode->append_attribute(doc.allocate_attribute("rectright", rectRight));

	//append this node to root node
	parentXmlNode->append_node(xmlNode);

	if (sdqNode->rect.top - sdqNode->rect.bottom < minSquareSize)
	{
		//append bodyid and tileid to this node
		for (std::vector<SDQObject>::iterator it = sdqNode->sdqObjects.begin(); it != sdqNode->sdqObjects.end(); ++it)
		{
			char* id = doc.allocate_string(std::to_string(it->id).c_str());
			if (it->isContainBody)
			{
				xmlNode->append_attribute(doc.allocate_attribute("bodyid", id));
			}
			else
			{
				xmlNode->append_attribute(doc.allocate_attribute("tileid", id));
			}
		}



		return;
	}
	
	if(sdqNode->sdqObjects.size() == 0)
	{
		return;
	}
	

	//top left
	sdqNode->tl = new SDQNode(sdqNode->id * 10 + 1, sdqNode->id);
	sdqNode->tl->rect.top = sdqNode->rect.top;
	sdqNode->tl->rect.left = sdqNode->rect.left;
	sdqNode->tl->rect.bottom = (sdqNode->rect.top + sdqNode->rect.bottom) / 2;
	sdqNode->tl->rect.right = (sdqNode->rect.left + sdqNode->rect.right) / 2;

	//top right
	sdqNode->tr = new SDQNode(sdqNode->id * 10 + 2, sdqNode->id);
	sdqNode->tr->rect.top = sdqNode->rect.top;
	sdqNode->tr->rect.left = (sdqNode->rect.left + sdqNode->rect.right) / 2;
	sdqNode->tr->rect.bottom = (sdqNode->rect.top + sdqNode->rect.bottom) / 2;
	sdqNode->tr->rect.right = sdqNode->rect.right;


	//bottom left
	sdqNode->br = new SDQNode(sdqNode->id * 10 + 3, sdqNode->id);
	sdqNode->br->rect.top = (sdqNode->rect.top + sdqNode->rect.bottom) / 2;
	sdqNode->br->rect.left = (sdqNode->rect.left + sdqNode->rect.right) / 2;
	sdqNode->br->rect.bottom = sdqNode->rect.bottom;
	sdqNode->br->rect.right = sdqNode->rect.right;


	//bottom right
	sdqNode->bl = new SDQNode(sdqNode->id * 10 + 4, sdqNode->id);
	sdqNode->bl->rect.top = (sdqNode->rect.top + sdqNode->rect.bottom) / 2;
	sdqNode->bl->rect.left = sdqNode->rect.left;
	sdqNode->bl->rect.bottom = sdqNode->rect.bottom;
	sdqNode->bl->rect.right = (sdqNode->rect.left + sdqNode->rect.right) / 2;

	Collision collision;

	for (std::vector<SDQObject>::iterator it = sdqNode->sdqObjects.begin(); it != sdqNode->sdqObjects.end(); ++it)
	{
		if (collision.IsOverlayingRect(it->rect, sdqNode->tl->rect))
		{
			sdqNode->tl->sdqObjects.push_back(*it);
		}

		if (collision.IsOverlayingRect(it->rect, sdqNode->tr->rect))
		{
			sdqNode->tr->sdqObjects.push_back(*it);
		}

		if (collision.IsOverlayingRect(it->rect, sdqNode->br->rect))
		{
			sdqNode->br->sdqObjects.push_back(*it);
		}

		if (collision.IsOverlayingRect(it->rect, sdqNode->bl->rect))
		{
			sdqNode->bl->sdqObjects.push_back(*it);
		}

	}

	//clear 
	sdqNode->sdqObjects.clear();

	//
	BuildTree(sdqNode->tl, xmlNode);
	BuildTree(sdqNode->tr, xmlNode);
	BuildTree(sdqNode->br, xmlNode);
	BuildTree(sdqNode->bl, xmlNode);
}

//
void SpaceDivisionQuadTree::EndBuildingTreeNodes()
{
	Release(rootSDQNode);

	// Save to file
	std::ofstream file_stored(outpath);
	file_stored << doc;
	file_stored.close();

	//clear for sure
	doc.clear();
}

void  SpaceDivisionQuadTree::Release(SDQNode* sdqNode)
{
	if (sdqNode == nullptr) return;

	Release(sdqNode->tl);
	Release(sdqNode->tr);
	Release(sdqNode->br);
	Release(sdqNode->bl);

	if (sdqNode->tl != nullptr)
	{
		sdqNode->tl->Release();
		delete sdqNode->tl;
		sdqNode->tl = nullptr;
	}

	if (sdqNode->tr != nullptr)
	{
		sdqNode->tr->Release();
		delete sdqNode->tr;
		sdqNode->tr = nullptr;
	}

	if (sdqNode->br != nullptr)
	{
		sdqNode->br->Release();
		delete sdqNode->br;
		sdqNode->br = nullptr;
	}

	if (sdqNode->bl != nullptr)
	{
		sdqNode->bl->Release();
		delete sdqNode->bl;
		sdqNode->bl = nullptr;
	}

}

//
void SpaceDivisionQuadTree::BuildTreeNodesFromTMX(const std::string &outpath, const std::string &tmxFilePath)
{
	
	StartBuildingTreeNodes(outpath, tmxFilePath);
	BuildTree(rootSDQNode, rootXmlNode);
	EndBuildingTreeNodes();

}



void SpaceDivisionQuadTree::Load(const std::string &quadtreeFilePath, const std::string &tmxFilePath)
{
	//load tmx file
	std::string tmxfileContents = "";
	if (LoadFile(tmxFilePath, tmxfileContents) == true)
	{
		// Create new RapidXML document instance to use to parse data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)tmxfileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("map");

		unsigned int mapwidth = atoi(parentNode->first_attribute("width")->value());
		unsigned int mapheight = atoi(parentNode->first_attribute("height")->value());
		unsigned int maptileWidth = atoi(parentNode->first_attribute("tilewidth")->value());
		unsigned int maptileHeight = atoi(parentNode->first_attribute("tileheight")->value());

		// Create a new node based on the parent node
		rapidxml::xml_node<> *currentNode = parentNode;

		// Move to first layer node
		currentNode = currentNode->first_node("objectgroup");

		while (currentNode != nullptr)
		{
			std::string groupName = currentNode->first_attribute("name")->value();
			std::vector<Body*> bodiesInGroup;

			//get object node
			rapidxml::xml_node<> *childNode = currentNode->first_node("object");
			while (childNode != nullptr)
			{
				float id = atoi(childNode->first_attribute("id")->value());
				float x = atoi(childNode->first_attribute("x")->value());
				float y = atoi(childNode->first_attribute("y")->value());
				float width = atoi(childNode->first_attribute("width")->value());
				float height = atoi(childNode->first_attribute("height")->value());

				Shape::Rectangle rect(x + width / 2, mapheight*maptileHeight - y - height / 2, width, height);
				rect.id = id;

				Body* body = new Body(rect.x, rect.y, rect.width, rect.height, 0, 0);
				body->categoryBits = 0;
				body->maskBits = 0;
				body->id = id;

				//if (world != NULL)
				//{
				//	//push to world
				//	world->AddSDQBody(body);

				//push to vector
				bodiesInGroup.push_back(body);

				/*}*/

				//push to map body
				mapBody[rect.id] = body;

				//next
				childNode = childNode->next_sibling("object");
			}

			//push to bodyGroups
			bodyGroups[groupName] = bodiesInGroup;

			// Move to the next objectGroup
			currentNode = currentNode->next_sibling("objectgroup");
		}

		//get tile data
		// Create a new node based on the parent node
		rapidxml::xml_node<> *layerNode = parentNode;
		// Move to first layer node
		layerNode = parentNode->first_node("layer");
		TMXTileLayer layer;
		if (layerNode != nullptr)
		{
			//get layer attributes
			unsigned int layerWidth = atoi(layerNode->first_attribute("width")->value());
			unsigned int layerHeight = atoi(layerNode->first_attribute("height")->value());

			//set layer attributes
			layer.SetAttributes("", layerWidth, layerHeight);

			//get layer data
			char* data = layerNode->first_node("data")->value();

			//set layer data
			layer.SetData(data);

		}

		//get tile set 
		// Create a new node based on the parent node
		currentNode = parentNode;
		//get tileset node
		currentNode = currentNode->first_node("tileset");
		unsigned int tileSetWidth = 0;
		unsigned int tileSetHeight = 0;
		unsigned int columns = 0;
		// Check if there is a tileset node
		if (currentNode != nullptr)
		{
			tileSetWidth = atoi(currentNode->first_attribute("tilewidth")->value());
			tileSetHeight = atoi(currentNode->first_attribute("tileheight")->value());
			columns = atoi(currentNode->first_attribute("columns")->value());
		}


		//load map tile rectangle
		unsigned int **data = layer.GetData();
		unsigned int layerWidth = layer.GetWidth();
		unsigned int layerHeight = layer.GetHeight();

		float x, y, rectImageLeft, rectImageTop;
		float width = tileSetWidth;
		float height = tileSetHeight;
		unsigned int id = -1;
		for (unsigned int row = 0; row < layerHeight; row++)
		{
			for (unsigned int column = 0; column < layerWidth; column++)
			{
				id++;
				if (data[row][column] == 0) continue;

				rectImageLeft = ((data[row][column] - 1) % columns) * width;
				rectImageTop = ((data[row][column] - 1) / columns) * height;

				x = column * width + width / 2;
				y = (layerHeight - 1 - row)*height + height / 2;
				Shape::Rectangle* rect = new Shape::Rectangle(x, y, width, height);
				rect->id = id;
				rect->extraX = rectImageLeft;
				rect->extraY = rectImageTop;
				mapTileRectangle[id] = rect;

			}
		}

	}

	// String to hold file contents
	std::string fileContents = "";
	if (LoadFile(quadtreeFilePath, fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("spacedivisionmap");


		//get sdq node
		rapidxml::xml_node<>* currentNode = parentNode->first_node("sdqnode");

		Load(currentNode);

		LinkNodes();
	}



}

void SpaceDivisionQuadTree::Load(rapidxml::xml_node<>* xmlnode)
{
	if (xmlnode == nullptr) return;

	SDQNode* sdqNode = new SDQNode();

	//get id of the node
	sdqNode->id = atoi(xmlnode->first_attribute("id")->value());

	//get parentid of the node
	sdqNode->parentID = atoi(xmlnode->first_attribute("parentid")->value());

	//get rect
	sdqNode->rect.top = atoi(xmlnode->first_attribute("recttop")->value());
	sdqNode->rect.left = atoi(xmlnode->first_attribute("rectleft")->value());
	sdqNode->rect.bottom = atoi(xmlnode->first_attribute("rectbottom")->value());
	sdqNode->rect.right = atoi(xmlnode->first_attribute("rectright")->value());


	//get all body ids
	rapidxml::xml_attribute<>* nodeBodyID = xmlnode->first_attribute("bodyid");

	while (nodeBodyID != nullptr)
	{
		unsigned int bodyID = atoi(nodeBodyID->value());
		sdqNode->sdqObjects.push_back(SDQObject(bodyID, mapBody[bodyID], NULL));

		//next attribute
		nodeBodyID = nodeBodyID->next_attribute("bodyid");
	}

	//get all tile rectangle ids
	rapidxml::xml_attribute<>* nodeTileID = xmlnode->first_attribute("tileid");

	while (nodeTileID != nullptr)
	{
		unsigned int tileID = atoi(nodeTileID->value());
		sdqNode->sdqObjects.push_back(SDQObject(tileID, NULL, mapTileRectangle[tileID]));

		//next attribute
		nodeTileID = nodeTileID->next_attribute("tileid");
	}

	//put this node into mapQuadtree
	mapQuadTree[sdqNode->id] = sdqNode;

	//Load for child node
	Load(xmlnode->first_node("sdqnode"));

	//Load for next sibling
	Load(xmlnode->next_sibling());


}

void SpaceDivisionQuadTree::LinkNodes()
{
	for (std::map<unsigned int, SDQNode*>::iterator it = mapQuadTree.begin(); it != mapQuadTree.end(); ++it)
	{
		if (it->first == 0)
		{
			rootSDQNode = it->second;
		}
		else
		{
			//get parentID
			unsigned int parentID = it->second->parentID;

			//Get parent node from ID
			SDQNode* parentNode = mapQuadTree[parentID];

			//get childID, which is current node
			unsigned int childID = it->first;
			switch (childID%10)
			{
			case 1:
				parentNode->tl = it->second;
				break;
			case 2:
				parentNode->tr = it->second;
				break;
			case 3:
				parentNode->bl = it->second;
				break;
			case 4:
				parentNode->br = it->second;
				break;
			}
		}

	}

}

const std::map<unsigned int, SDQNode*>& SpaceDivisionQuadTree::GetMapQuadTree()
{
	return mapQuadTree;
}

const std::map<unsigned int, Body*>& SpaceDivisionQuadTree::GetMapBody()
{
	return mapBody;
}

//SDQNode* SpaceDivisionQuadTree::GetRootSDQNode()
//{
//	return rootSDQNode;
//}

void SpaceDivisionQuadTree::LoadObjectsInViewport(Camera* camera, bool loadBody, bool loadTileRect)
{
	RECT viewportRect;
	viewportRect.top = camera->GetPosition().y+ screenHeight /2;
	viewportRect.left = camera->GetPosition().x - screenWidth / 2;
	viewportRect.bottom = camera->GetPosition().y - screenHeight / 2;
	viewportRect.right = camera->GetPosition().x + screenWidth / 2;

	this->loadBody = loadBody;
	this->loadTileRect = loadTileRect;

	LoadObjectsInViewport(viewportRect, rootSDQNode);

}

void SpaceDivisionQuadTree::LoadObjectsInViewport(const RECT &viewport, SDQNode* sdqNode)
{
	if (sdqNode == nullptr) return;

	if (sdqNode == rootSDQNode)
	{
		bodiesInViewport.clear();
		tileRectsInViewport.clear();
	}

	Collision collision;
	if (sdqNode->tl == nullptr) //leaf node
	{
		if (collision.IsOverlayingRect(sdqNode->rect, viewport))
		{
			for (std::vector<SDQObject>::iterator it = sdqNode->sdqObjects.begin(); it != sdqNode->sdqObjects.end(); ++it)
			{
				Body *body = it->body;
				if (it->body != NULL && it->body->categoryBits!=0)
				{
					if (!loadBody) continue;

					//find this body in bodiesInViewport
					std::vector<Body*>::iterator itBody = std::find(bodiesInViewport.begin(), bodiesInViewport.end(), body);
					if (itBody != bodiesInViewport.end()) //if this vector contains current body
					{
						continue;
					}
					else
					{
						bodiesInViewport.push_back(body);
					}
				}
				else
				{
					if (it->tileRectangle != NULL)
					{
						if (!loadTileRect) continue;

						//find this body in bodiesInViewport
						std::vector<Shape::Rectangle*>::iterator itRect = std::find(tileRectsInViewport.begin(), tileRectsInViewport.end(), it->tileRectangle);
						if (itRect != tileRectsInViewport.end()) //if this vector contains current body
						{
							continue;
						}
						else
						{
							tileRectsInViewport.push_back(it->tileRectangle);
						}
					}
				}

			}
		}
	}
	else
	{
		if (collision.IsOverlayingRect(sdqNode->tl->rect, viewport))
		{
			LoadObjectsInViewport(viewport, sdqNode->tl);
		}
		if (collision.IsOverlayingRect(sdqNode->tr->rect, viewport))
		{
			LoadObjectsInViewport(viewport, sdqNode->tr);
		}
		if (collision.IsOverlayingRect(sdqNode->br->rect, viewport))
		{
			LoadObjectsInViewport(viewport, sdqNode->br);
		}
		if (collision.IsOverlayingRect(sdqNode->bl->rect, viewport))
		{
			LoadObjectsInViewport(viewport, sdqNode->bl);
		}
	}

}

const std::vector<Body*>&  SpaceDivisionQuadTree::GetBodiesInViewport()
{
	return bodiesInViewport;
}

const std::vector<Shape::Rectangle*>&  SpaceDivisionQuadTree::GetTileRectsInViewport()
{
	return tileRectsInViewport;
}

const std::map<unsigned int, Shape::Rectangle*>& SpaceDivisionQuadTree::GetMapTileRect()
{
	return mapTileRectangle;
}

//void SpaceDivisionQuadTree::SetWorld(World *world)
//{
//	this->world = world;
//}

const std::vector<Body*>& SpaceDivisionQuadTree::GetBodiesGroup(const std::string &groupName)
{
	// Attempt to find and return a vector using provided name, else return nullptr
	std::map<std::string, std::vector<Body*>>::const_iterator it = bodyGroups.find(groupName);

	if (it == bodyGroups.end())
	{
		std::vector<Body*> nullVector;
		return nullVector;
	}
	else
	{
		//first means key, which is mapName
		//second means value, which is TMXMap
		return it->second;
	}
}