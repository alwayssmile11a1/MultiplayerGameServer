#include "TexturePacker.h"

TexturePacker::TexturePacker()
{
	_Content.clear();
}

TexturePacker::~TexturePacker()
{

}

TexturePacker::TexturePacker(Texture* texture, std::string packerfilePath)
{
	LoadFile(packerfilePath, _Content);

	_Texture = texture;;

}

std::vector<TextureRegion> TexturePacker::GetRegion(std::string regionName) const
{
	std::vector<TextureRegion> regions;
	
	////get dimensions represented by a string
	//std::string result = "";
	//std::size_t first = _Content.find("<" + regionName + ">");
	//std::size_t last = _Content.find("</" + regionName + ">");

	//if (first != std::string::npos && last != std::string::npos)
	//{
	//	result = _Content.substr(first + 3 + regionName.size(), last - first - 4 - regionName.size());
	//}
	//
	//std::string::size_type sz;     // alias of size_t

	//while (result != "")
	//{
	//	//get rectLeft
	//	float rectLeft = std::stof(result, &sz);
	//	result = result.substr(sz);

	//	//get rectTop
	//	float rectTop = std::stof(result, &sz);
	//	result = result.substr(sz);

	//	//get rectWidth
	//	float rectWidth = std::stof(result, &sz);
	//	result = result.substr(sz);

	//	//get rectHeight
	//	float rectHeight = std::stof(result, &sz);
	//	result = result.substr(sz);
	//	
	//	TextureRegion textureRegion(_Texture, rectLeft, rectTop, rectWidth, rectHeight);
	//	regions.push_back(textureRegion);

	//}

	std::string tempContent = _Content;

	if (!tempContent._Equal(""))
	{
		// Create new RapidXML document instance to use to parse map data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)tempContent.c_str());

		//get region node
		rapidxml::xml_node<> *currentNode = currentMap.first_node((char*)regionName.c_str());

		//get rect node
		currentNode = currentNode->first_node("rect");

		while (currentNode != nullptr)
		{
			float rectLeft = atoi(currentNode->first_attribute("left")->value());

			//get rectTop
			float rectTop = atoi(currentNode->first_attribute("top")->value());

			//get rectWidth
			float rectWidth = atoi(currentNode->first_attribute("width")->value());


			//get rectHeight
			float rectHeight = atoi(currentNode->first_attribute("height")->value());


			TextureRegion textureRegion(_Texture, rectLeft, rectTop, rectWidth, rectHeight);
			regions.push_back(textureRegion);

			currentNode = currentNode->next_sibling();

		}
	}

	return regions;


}

void TexturePacker::SetTexture(Texture *texture)
{
	_Texture = texture;
}

void TexturePacker::SetPacker(std::string packerfilePath)
{
	_Content.clear();
	////get file
	//std::ifstream textureFile(packerfilePath);
	//std::string line;

	////get the content of this file
	//if (textureFile.is_open())
	//{
	//	while (getline(textureFile, line))
	//	{
	//		_Content.append(line + " ");
	//	}
	//	textureFile.close();
	//}

	LoadFile(packerfilePath, _Content);

}