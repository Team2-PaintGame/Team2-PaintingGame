#pragma once
#ifdef __ORBIS__
#include "AssetLoader.h"

namespace NCL::Assets {
	class GNMAssetLoader : public AssetLoader {
	public:
		GNMAssetLoader();
		virtual MeshGeometry* LoadMesh(const string& name) override;
		virtual MeshGeometry* LoadMesh(const MeshType& meshType) override;
		virtual ShaderBase* LoadShader(const string& vertex, const string& fragment) override;
		virtual MeshGeometry* LoadFlatMesh(int hVertexCount = 128, int wVertexCount = 128) override;
		virtual MeshGeometry* LoadHeightMap(const std::string& filename, int heightMultiplier = 10) override;
	};

	// Concrete factory for creating OpenGL Asset Loader
	class GNMAssetLoaderFactory : public AssetLoaderFactory {
	public:
		AssetLoader* createAssetLoader() override {
			return new GNMAssetLoader();
		}
	};	
}
#endif