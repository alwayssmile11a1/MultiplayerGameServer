#ifndef TMXLOADER_H
#define TMXLOADER_H


#include <unordered_map>
#include "..\extensions\rapidxml\rapidxml.hpp"
#include "TMXMap.h"
#include "TMXTileLayer.h"
#include "TMXTileSet.h"
#include "..\src\others\Utility.h"


class TMXLoader
{
private:

	//use unordered_map for fast access purpose
	std::unordered_map<std::string, TMXMap*> _MapContainer;


	//private functions
	void LoadMapSettings(TMXMap* map, rapidxml::xml_node<> *parentNode);
	void LoadTileSets(TMXMap* map, rapidxml::xml_node<> *parentNode, const std::string& folderPath);
	void LoadLayers(TMXMap* map, rapidxml::xml_node<> *parentNode);
	void LoadObjectGroups(TMXMap* map, rapidxml::xml_node<> *parentNode);

	float _ScaleFactor;

public:
	TMXLoader();
	~TMXLoader();

	void AddMap(const std::string& mapName, const std::string& filePath, float scale);
	TMXMap* GetMap(const std::string& mapName) const;

};

#endif