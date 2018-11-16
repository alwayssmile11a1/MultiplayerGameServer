#ifndef TMXTILESET_H
#define TMXTILESET_H

#include <string>
#include "..\drawable\Texture.h"

class TMXTileSet
{
private:
	std::string _Name;
	unsigned int _FirstGid;
	unsigned int _TileWidth;
	unsigned int _TileHeight;
	unsigned int _TileCount;
	unsigned int _Columns;
	std::string _SourcePath;
	unsigned int _ImageWidth;
	unsigned int _ImageHeight;
	Texture *_Texture;
	
	void SetAttributes(const std::string& name, unsigned int firstGid, unsigned int tileWidth, unsigned int tileHeight,
		unsigned int tileCount, unsigned int columns, const std::string& sourcePath, unsigned int imageWidth, unsigned int imageHeight);

	friend class TMXLoader;

public:
	TMXTileSet();
	~TMXTileSet();
	TMXTileSet(const TMXTileSet &tmxTileSet);
	TMXTileSet& operator=(const TMXTileSet &tmxTileSet);


	const std::string& GetName() const;
	const std::string& GetSourcePath() const;
	unsigned int GetColumns() const;
	unsigned int GetFirstGid() const;
	unsigned int GetImageWidth() const;
	unsigned int GetImageHeight() const;
	unsigned int GetTileWidth() const;
	unsigned int GetTileHeight() const;
	unsigned int GetTileCount() const;
	
	Texture	*GetTexture() const;

};

#endif