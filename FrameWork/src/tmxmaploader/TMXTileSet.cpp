#include "TMXTileSet.h"

TMXTileSet::TMXTileSet()
{
	_FirstGid = 1;
	_TileWidth = 0;
	_TileHeight = 0;
	_TileCount = 0;
	_Columns = 0;
	_ImageWidth = 0;
	_ImageHeight = 0;
	_Texture = NULL;
}

void TMXTileSet::SetAttributes(const std::string& name, unsigned int firstGid, unsigned int tileWidth, unsigned int tileHeight,
	 unsigned int tileCount, unsigned int columns, const std::string& sourcePath, unsigned int imageWidth, unsigned int imageHeight)
{
	_FirstGid = firstGid;
	_Name = name;
	_SourcePath = sourcePath;
	_ImageWidth = imageWidth;
	_ImageHeight = imageHeight;
	_TileWidth = tileWidth;
	_TileHeight = tileHeight;
	_TileCount = tileCount;
	_Columns = columns;

	_Texture = new Texture(_SourcePath);
}

TMXTileSet::~TMXTileSet()
{
	if (_Texture != NULL)
	{
		delete _Texture;
		_Texture = NULL;
	}
}

TMXTileSet::TMXTileSet(const TMXTileSet &tmxTileSet)
{
	_FirstGid = tmxTileSet._FirstGid;
	_Name = tmxTileSet._Name;
	_SourcePath = tmxTileSet._SourcePath;
	_ImageWidth = tmxTileSet._ImageWidth;
	_ImageHeight = tmxTileSet._ImageHeight;
	_TileWidth = tmxTileSet._TileWidth;
	_TileHeight = tmxTileSet._TileHeight;
	_TileCount = tmxTileSet._TileCount;
	_Columns = tmxTileSet._Columns;

	_Texture = new Texture();
	*_Texture = *tmxTileSet._Texture;
}
TMXTileSet& TMXTileSet::operator=(const TMXTileSet &tmxTileSet)
{
	_FirstGid = tmxTileSet._FirstGid;
	_Name = tmxTileSet._Name;
	_SourcePath = tmxTileSet._SourcePath;
	_ImageWidth = tmxTileSet._ImageWidth;
	_ImageHeight = tmxTileSet._ImageHeight;
	_TileWidth = tmxTileSet._TileWidth;
	_TileHeight = tmxTileSet._TileHeight;
	_TileCount = tmxTileSet._TileCount;
	_Columns = tmxTileSet._Columns;

	_Texture = new Texture();
	*_Texture = *tmxTileSet._Texture;

	return *this;

}

Texture* TMXTileSet::GetTexture() const
{
	return _Texture;
}

const std::string& TMXTileSet::GetName() const
{
	return _Name;
}


const std::string& TMXTileSet::GetSourcePath() const
{
	return _SourcePath;
}


unsigned int TMXTileSet::GetFirstGid() const
{
	return _FirstGid;
}

unsigned int TMXTileSet::GetImageWidth() const
{
	return _ImageWidth;
}


unsigned int TMXTileSet::GetImageHeight() const
{
	return _ImageHeight;
}


unsigned int TMXTileSet::GetTileWidth() const
{
	return _TileWidth;
}


unsigned int TMXTileSet::GetTileHeight() const
{
	return _TileHeight;
}

 
unsigned int TMXTileSet::GetTileCount() const
{
	return _TileCount;
}

unsigned int TMXTileSet::GetColumns() const
{
	return _Columns;
}