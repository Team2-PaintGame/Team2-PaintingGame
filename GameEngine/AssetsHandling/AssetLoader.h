/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <string>
#include "TextureBase.h"
#include "ShaderBase.h"
#include "MeshGeometry.h"
#include "TextureLoader.h"

namespace NCL::Assets {
	using namespace Rendering;
	enum class MeshType {
		Quad,
		Triangle,
	};
	class AssetLoader {
	public:
		virtual TextureBase* LoadTexture(const string& name) { return TextureLoader::LoadAPITexture(name); }
		virtual MeshGeometry* LoadMesh(const string& name)												= 0;
		virtual MeshGeometry* LoadMesh(const MeshType& meshType)										= 0;
		virtual ShaderBase* LoadShader(const string& vertex, const string& fragment)					= 0;
		virtual MeshGeometry* LoadFlatMesh(int hVertexCount = 128, int wVertexCount = 128)				= 0;
		virtual MeshGeometry* LoadHeightMap(const std::string& filename, int heightMultiplier = 10)		= 0;
	};

	class AssetLoaderFactory {
	public:
		virtual ~AssetLoaderFactory() {}
		virtual AssetLoader* createAssetLoader() = 0;
	};
}