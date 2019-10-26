#include "Texture.h"

Texture::Texture()
{
	//set
	_Transcolor = D3DCOLOR_ARGB(255,255, 255, 255);
	_ImageSize.Set(0, 0);
	_Image = NULL;
}

Texture::Texture(const std::string &filePath)
{
	//set basic information
	_Transcolor = D3DCOLOR_ARGB(0, 0, 0, 0);
	_FilePath = filePath;
	_Image = NULL;

	//get path from string
	std::wstring stemp = StringToWstring(filePath);
	LPCWSTR path = stemp.c_str();

	//Get image from file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(path, &info);
	if (result != D3D_OK)
	{
		return;
	}

	//set size
	_ImageSize.Set(info.Width, info.Height);

	result = D3DXCreateTextureFromFileEx(
		d3ddevice,
		path,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_Transcolor,
		&info,
		NULL,
		&_Image);


	if (result != D3D_OK)
	{
		return;
	}


}


Texture::Texture(const Texture &texture)
{
	_Transcolor = texture._Transcolor;
	_ImageSize = texture._ImageSize;
	_FilePath = texture._FilePath;

	//get path from string
	std::wstring stemp = StringToWstring(_FilePath);
	LPCWSTR path = stemp.c_str();

	//Get image from file
	_Image = NULL;
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(path, &info);
	D3DXCreateTextureFromFileEx(
		d3ddevice,
		path,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_Transcolor,
		&info,
		NULL,
		&_Image);

}

Texture& Texture::operator=(const Texture &texture)
{
	_Transcolor = texture._Transcolor;
	_ImageSize = texture._ImageSize;	
	_FilePath = texture._FilePath;
	
	//get path from string
	std::wstring stemp = StringToWstring(_FilePath);
	LPCWSTR path = stemp.c_str();

	//Get image from file
	_Image = NULL;
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(path, &info);
	D3DXCreateTextureFromFileEx(
		d3ddevice,
		path,
		info.Width,
		info.Height,
		1,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		_Transcolor,
		&info,
		NULL,
		&_Image);


	return *this;
}


////Create a clone of this texture
//void Texture::Clone(Texture &texture)
//{
//		texture._Transcolor = _Transcolor;
//		texture._ImageSize = _ImageSize;
//		texture._FilePath = _FilePath;
//		
//		//get path from string
//		std::wstring stemp = StringToWstring(_FilePath);
//		LPCWSTR path = stemp.c_str();
//	
//		//Get image from file
//		texture._Image = NULL;
//		D3DXIMAGE_INFO info;
//		D3DXGetImageInfoFromFile(path, &info);
//		D3DXCreateTextureFromFileEx(
//			d3ddevice,
//			path,
//			info.Width,
//			info.Height,
//			1,
//			D3DPOOL_DEFAULT,
//			D3DFMT_UNKNOWN,
//			D3DPOOL_DEFAULT,
//			D3DX_DEFAULT,
//			D3DX_DEFAULT,
//			_Transcolor,
//			&info,
//			NULL,
//			&texture._Image);
//	
//}
//
////release this texture
//void Texture::Release()
//{
//	if (_Image != NULL)
//	{
//		_Image->Release();
//		_Image = NULL;
//	}
//}

//all get functions
D3DCOLOR Texture::GetTranscolor() const
{
	return _Transcolor;
}

LPDIRECT3DTEXTURE9 Texture::GetImage() const
{
	return _Image;
}

//all set functions
void Texture::SetTranscolor(D3DCOLOR transcolor)
{
	_Transcolor = transcolor;
}

const Vector2& Texture::GetImageSize() const
{
	return _ImageSize;
}


Texture::~Texture()
{

	if (_Image != NULL)
	{
		_Image->Release();
		_Image = NULL;
	}

}
