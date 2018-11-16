#ifndef TEXTURE_H
#define TEXTURE_H

#include "GameObject.h"
#include "..\others\Global.h"
#include "..\math\Vector2.h"
#include "..\others\Utility.h"

//Just like a image
class Texture: public GameObject
{
private:
	LPDIRECT3DTEXTURE9 _Image; //store the image that was loaded from file
	std::string _FilePath;
	D3DCOLOR _Transcolor; //what color is needed to be transparent?
	Vector2 _ImageSize; //the whole width and height of the image


public:
	Texture();

	//create a texture from file
	Texture(const std::string &filePath);
	~Texture();

	////copy constructor - this is really important 
	////"The copy constructor is a constructor which creates an object by initializing it with an object of the same class, 
	////which has been created previously. The copy constructor is used to −
	////Initialize one object from another of the same type.
	////Copy an object to pass it as an argument to a function.
	////Copy an object to return it from a function.
	Texture(const Texture &texture);
	Texture& operator=(const Texture &texture);

	//all get functions
	D3DCOLOR GetTranscolor() const;
	LPDIRECT3DTEXTURE9 GetImage() const;
	const Vector2& GetImageSize() const;

	//all set functions
	void SetTranscolor(D3DCOLOR transcolor);

	////Create a clone of this texture
	//void Clone(Texture &texture);
	//
	////release this texture
	//void Release();

};

#endif

