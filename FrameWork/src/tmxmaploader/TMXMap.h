#ifndef TMXMAP_H
#define TMXMAP_H

#include <vector>
#include "TMXTileSet.h"
#include "TMXTileLayer.h"
#include "TMXObjectGroup.h"
#include "..\drawable\SpriteBatch.h"
#include <unordered_map>
#include "..\spacedivisionquadtree\SpaceDivisionQuadTree.h"

class TMXMap
{
private:
	unsigned int _Width;
	unsigned int _Height;
	unsigned int _TileWidth;
	unsigned int _TileHeight;
	
	TMXTileSet *_TileSet;
	std::unordered_map<std::string, TMXTileLayer*> _Layers;
	std::unordered_map<std::string, TMXObjectGroup*> _ObjectGroups;

	//Camera* _Cam; //hold camera reference

	float _ScaleFactor;

	SpaceDivisionQuadTree* _SDQuadTree;

	void SetAttributes(unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight);
	void SetTileSet(const TMXTileSet &tileSet);
	void AddLayer(const std::string &layerName, const TMXTileLayer &layer);
	void AddObjectGroup(const std::string &name, const TMXObjectGroup &objectGroup);

	friend class TMXLoader;

public:
	TMXMap();
	~TMXMap();

	////set camera to reduce the amount of tiles that have to be loaded 
	//void SetCamera(Camera* camera);


	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetTileWidth() const;
	unsigned int GetTileHeight() const;

	TMXTileSet *GetTileSet() const;
	const std::unordered_map<std::string, TMXTileLayer*>& GetLayers() const;
	const std::unordered_map<std::string, TMXObjectGroup*>& GetObjectGroups() const;

	TMXObjectGroup* GetObjectGroup(const std::string &groupName) const;
	TMXTileLayer* GetTileLayer(const std::string &tileLayerName) const;

	//Render this map
	void Render(SpriteBatch *batch);

	void SetScale(float scale);
	float GetScale();

	//set this to reduce the amount of tile have to render
	void SetSpaceDivisionQuadTree(SpaceDivisionQuadTree *sdquadtree);

	void SetTileData(int row, int column, int value);



};

#endif