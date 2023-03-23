/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "TextureLoader.h"
#include <iostream>
#ifdef _WIN32
#include <filesystem>
#endif 
#define STB_IMAGE_IMPLEMENTATION

#include "./stb/stb_image.h"

#include "Assets.h"

using namespace NCL;
using namespace Rendering;

std::map<std::string, TextureLoadFunction> TextureLoader::fileHandlers;
APILoadFunction TextureLoader::apiFunction = nullptr;

bool TextureLoader::LoadTexture(const std::string& filename, char*& outData, int& width, int &height, int &channels, int&flags) {
	if (filename.empty()) {
		return false;
	}
#ifdef _WIN32
	std::filesystem::path path(filename);

	std::string extension = path.extension().string();

	bool isAbsolute = path.is_absolute();

	auto it = fileHandlers.find(extension);

	std::string realPath = isAbsolute ? filename : Assets::TEXTUREDIR + filename;
#else
	std::string extension = GetFileExtension(filename);

	auto it = fileHandlers.find(extension);

	std::string realPath = Assets::TEXTUREDIR + filename;
#endif 

	if (it != fileHandlers.end()) {
		//There's a custom handler function for this, just use that
		return it->second(realPath, outData, width, height, channels, flags);
	}
	//By default, attempt to use stb image to get this texture
	stbi_uc *texData = stbi_load(realPath.c_str(), &width, &height, &channels, 4); //4 forces this to always be rgba!

	channels = 4; //it gets forced, we don't care about the 'real' channel size

	if (texData) {
		outData = (char*)texData;
		return true;
	}

	return false;
}

bool TextureLoader::LoadTextureGreyScale(const std::string& filename, unsigned char*& outData, int& width, int& height, int& channels, int& flags) {
	if (filename.empty()) {
		return false;
	}

#ifdef _WIN32
	std::filesystem::path path(filename);
	bool isAbsolute = path.is_absolute();
	std::string realPath = isAbsolute ? filename : Assets::TEXTUREDIR + filename;
#else
	std::string extension = GetFileExtension(filename);

	std::string realPath = Assets::TEXTUREDIR + filename;
#endif 




	//By default, attempt to use stb image to get this texture
	unsigned char* texData = stbi_load(realPath.c_str(), &width, &height, &channels, 1); //1 forces this to always be grey scale!

	channels = 1; //it gets forced, we don't care about the 'real' channel size

	if (texData) {
		outData = texData;
		return true;
	}

	return false;
}

bool TextureLoader::FreeTexture(char*& texData) {
	if (texData) {
		stbi_image_free(texData);
		return true;
	}
	return false;
}

void TextureLoader::RegisterTextureLoadFunction(TextureLoadFunction f, const std::string&fileExtension) {
	fileHandlers.insert(std::make_pair(fileExtension, f));
}

std::string TextureLoader::GetFileExtension(const std::string& fileExtension) {
#ifdef _WIN32

	auto p = std::filesystem::path(fileExtension);
	auto ext = p.extension();
	return ext.string();

#else
	return std::string();
#endif
}

void TextureLoader::RegisterAPILoadFunction(APILoadFunction f) {
	if (apiFunction) {
		std::cout << __FUNCTION__ << " replacing previously defined API function.\n";
	}
	apiFunction = f;
}

TextureBase* TextureLoader::LoadAPITexture(const std::string&filename) {
	if (apiFunction == nullptr) {
		std::cout << __FUNCTION__ << " no API Function has been defined!\n";
		return nullptr;
	}
	return apiFunction(filename);
}