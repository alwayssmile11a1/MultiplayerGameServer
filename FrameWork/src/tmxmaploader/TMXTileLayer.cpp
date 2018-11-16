#include "TMXTileLayer.h"

TMXTileLayer::TMXTileLayer()
{
	_Width = 0;
	_Height = 0;
	_Data = NULL;
}

TMXTileLayer::~TMXTileLayer()
{
	for (unsigned int i = 0; i < _Height; i++) {
		delete[] _Data[i];
	}
	delete[] _Data;
}

TMXTileLayer::TMXTileLayer(const TMXTileLayer &tmxTileLayer)
{
	_LayerName = tmxTileLayer._LayerName;
	_Width = tmxTileLayer._Width;
	_Height = tmxTileLayer._Height;

	//copy data
	_Data = new unsigned int*[_Height];
	for (unsigned int row = 0; row < _Height; row++)
	{
		_Data[row] = new unsigned int[_Width];

		for (unsigned int column = 0; column < _Width; column++)
		{
			_Data[row][column] = tmxTileLayer._Data[row][column];

		}
	}
	
}
TMXTileLayer& TMXTileLayer::operator=(const TMXTileLayer &tmxTileLayer)
{
	_LayerName = tmxTileLayer._LayerName;
	_Width = tmxTileLayer._Width;
	_Height = tmxTileLayer._Height;

	//copy data
	_Data = new unsigned int*[_Height];
	for (unsigned int row = 0; row < _Height; row++)
	{
		_Data[row] = new unsigned int[_Width];

		for (unsigned int column = 0; column < _Width; column++)
		{
			_Data[row][column] = tmxTileLayer._Data[row][column];
		
		}
	}

	return *this;
}


void TMXTileLayer::SetAttributes(const std::string& name, unsigned int width, unsigned int height)
{
	_LayerName = name;
	_Width = width;
	_Height = height;

}

void  TMXTileLayer::TMXTileLayer::SetData(char* matrixCSV)
{
	_Data = new unsigned int* [_Height];

	for (unsigned int i = 0; i < _Height; i++)
	{
		_Data[i] = new unsigned int[_Width];
	}

	char* context = NULL;
	char *separators = " \t,\n\r";

	//get data (convert matrixCSV to int matrix)
	char* item = strtok_s(matrixCSV, separators,&context);

	for (unsigned int row = 0; row < _Height; row++)
	{
		for (unsigned int column = 0; column < _Width; column++)
		{
			_Data[row][column] = atoi(item);
			item = strtok_s(0, separators, &context);
		}
	}

}



const std::string& TMXTileLayer::GetName() const
{
	return _LayerName;
}


unsigned int TMXTileLayer::GetWidth() const
{
	return _Width;
}


unsigned int TMXTileLayer::GetHeight() const
{
	return _Height;
}

unsigned int** TMXTileLayer::GetData() const
{ 
	return _Data;
}