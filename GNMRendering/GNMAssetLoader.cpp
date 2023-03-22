#ifdef __ORBIS__
#include "GNMAssetLoader.h"
#include "GNMTexture.h"
#include "GNMMesh.h"
#include "GNMShader.h"

using namespace NCL;
using namespace GNM;
using namespace Assets;
	
GNMAssetLoader::GNMAssetLoader() {
	TextureLoader::RegisterAPILoadFunction(GNMTexture::RGBATextureFromFilename);
}

MeshGeometry* GNMAssetLoader::LoadMesh(const string& name) {
	GNMMesh* mesh = new GNMMesh(name);
	mesh->UploadToGPU();
	return mesh;
}

MeshGeometry* GNMAssetLoader::LoadMesh(const MeshType& meshType) {
	switch (meshType) {
	case MeshType::Quad:
		return GNMMesh::GenerateQuad();
	default:
		return nullptr;
	}
}

ShaderBase* GNMAssetLoader::LoadShader(const string& vertex, const string& fragment) {
	return GNMShader::GenerateShader(vertex, fragment);
}

MeshGeometry* GNMAssetLoader::LoadFlatMesh(int hVertexCount, int wVertexCount) {
	return GNMMesh::GenerateFlatMesh(hVertexCount, wVertexCount);
}

MeshGeometry* GNMAssetLoader::LoadHeightMap(const std::string& filename, int heightMultiplier) {
	return GNMMesh::GenerateHeightMap(filename, heightMultiplier);
}
#endif
