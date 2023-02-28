#ifdef _WIN32
#include "AssetLoader.h"
#include "OGLAssetLoader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "OGLShader.h"

using namespace NCL;
using namespace Assets;
	
OGLAssetLoader::OGLAssetLoader() {
	TextureLoader::RegisterAPILoadFunction(OGLTexture::RGBATextureFromFilename);
}

MeshGeometry* OGLAssetLoader::LoadMesh(const string& name) {
	OGLMesh* mesh = new OGLMesh(name);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	mesh->UploadToGPU();
	return mesh;
}

MeshGeometry* OGLAssetLoader::LoadMesh(const MeshType& meshType) {
	switch (meshType) {
	case MeshType::Quad:
		return OGLMesh::GenerateQuad();
	default:
		return nullptr;
	}
}

ShaderBase* OGLAssetLoader::LoadShader(const string& vertex, const string& fragment) {
	return new OGLShader(vertex, fragment);
}

MeshGeometry* OGLAssetLoader::LoadFlatMesh(int hVertexCount, int wVertexCount) {
	return OGLMesh::GenerateFlatMesh(hVertexCount, wVertexCount);
}

MeshGeometry* OGLAssetLoader::LoadHeightMap(const std::string& filename, int heightMultiplier) {
	return OGLMesh::GenerateHeightMap(filename, heightMultiplier);
}
#endif
