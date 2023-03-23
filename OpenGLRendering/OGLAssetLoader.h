#pragma once
#ifdef _WIN32
#include "AssetLoader.h"

namespace NCL::Assets {
	class OGLAssetLoader : public AssetLoader {
	public:
		OGLAssetLoader();
		virtual MeshGeometry* LoadMesh(const string& name);
		virtual MeshGeometry* LoadMesh(const MeshType& meshType);
		virtual ShaderBase*	LoadShader(const string& vertex, const string& fragment);
		virtual MeshGeometry* LoadFlatMesh(int hVertexCount = 128, int wVertexCount = 128);
		virtual MeshGeometry* LoadHeightMap(const std::string& filename, int heightMultiplier = 10);
	};

	// Concrete factory for creating OpenGL Asset Loader
	class OGLAssetLoaderFactory : public AssetLoaderFactory {
	public:
		AssetLoader* createAssetLoader() override {
			return new OGLAssetLoader();
		}
	};

	
}
#endif