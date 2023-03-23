/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <string>

namespace NCL::Assets {
	//const std::string ASSETROOT("../../Assets/");
#ifdef _WIN32
	const std::string SHADERDIR(ASSETROOTLOCATION"Shaders/GL/");
#endif // _WIN32
#ifdef __ORBIS__
	const std::string SHADERDIR(ASSETROOTLOCATION"Shaders/GNM/");
#endif // __ORBIS__
	const std::string MESHDIR(ASSETROOTLOCATION"Meshes/");
	const std::string TEXTUREDIR(ASSETROOTLOCATION"Textures/");
	const std::string SOUNDSDIR(ASSETROOTLOCATION"Sounds/");
	const std::string FONTSSDIR(ASSETROOTLOCATION"Fonts/");
	const std::string DATADIR(ASSETROOTLOCATION"Data/");

	extern bool ReadTextFile(const std::string& filepath, std::string& result);
	extern bool ReadBinaryFile(const std::string& filepath, char** into, size_t& size);
}