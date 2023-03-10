#pragma once
#ifdef _ORBIS
#include "AssetLoader.h"
#include "GNMAssetLoader.h"
#include "GNMTexture.h"
#include "GNMMesh.h"
#include "GNMShader.h"

using namespace NCL;
using namespace Assets;
	
GNMAssetLoader::GNMAssetLoader() {
	TextureLoader::RegisterAPILoadFunction(GNMTexture::LoadTextureFromFile);
}

MeshGeometry* GNMAssetLoader::LoadMesh(const string& name) {
	GNMMesh* mesh = new GNMMesh(name);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
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
	return new GNMShader(vertex, fragment);
}

MeshGeometry* GNMAssetLoader::LoadFlatMesh(int hVertexCount, int wVertexCount) {
	return GNMMesh::GenerateFlatMesh(hVertexCount, wVertexCount);
}

MeshGeometry* GNMAssetLoader::LoadHeightMap(const std::string& filename, int heightMultiplier) {
	return GNMMesh::GenerateHeightMap(filename, heightMultiplier);
}
#endif
