/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#ifdef _WIN32
#include "OGLMesh.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Maths.h"
#include "TextureLoader.h";
#include "stb/stb_image.h"

using namespace NCL;
using namespace NCL::Rendering;
using namespace NCL::Maths;

OGLMesh::OGLMesh() {
	vao			= 0;
	subCount	= 1;

	for (int i = 0; i < VertexAttribute::MAX_ATTRIBUTES; ++i) {
		attributeBuffers[i] = 0;
	}
	indexBuffer = 0;
}

OGLMesh::OGLMesh(const std::string&filename) : MeshGeometry(filename){
	vao		 = 0;
	subCount = 1;

	for (int i = 0; i < VertexAttribute::MAX_ATTRIBUTES; ++i) {
		attributeBuffers[i] = 0;
	}
	indexBuffer = 0;
}

OGLMesh* OGLMesh::GenerateFlatMesh(int hVertexCount, int wVertexCount) {
	OGLMesh* m = new OGLMesh();
	m->primType = Triangles;

	for (int z = 0; z < hVertexCount; ++z) {
		for (int x = 0; x < wVertexCount; ++x) {
			m->positions.emplace_back(Vector3((float)x / ((float)wVertexCount - 1), 0, (float)z / ((float)hVertexCount - 1)));
			m->texCoords.emplace_back(Vector2((float)x / ((float)wVertexCount - 1), (float)z / ((float)hVertexCount - 1)));
		}
	}

	int i = 0;
	for (int z = 0; z < hVertexCount - 1; ++z) {
		for (int x = 0; x < wVertexCount - 1; ++x) {
			int a = (z * (wVertexCount)) + x;
			int b = (z * (wVertexCount)) + (x + 1);
			int c = ((z + 1) * (wVertexCount)) + (x + 1);
			int d = ((z + 1) * (wVertexCount)) + x;
			m->indices.emplace_back(a);
			m->indices.emplace_back(c);
			m->indices.emplace_back(b);
			m->indices.emplace_back(c);
			m->indices.emplace_back(a);
			m->indices.emplace_back(d);
		}
	}
	
	m->AddSubMesh(0, (wVertexCount - 1) * (hVertexCount - 1) * 6, 0);
	m->CalculateNormals();
	m->CalculateTangents();

	m->UploadToGPU();
	return m;
}

OGLMesh* OGLMesh::GenerateHeightMap(const std::string& filename, int heightMultiplier) {
	unsigned char* texData = nullptr;
	int wVertexCount = 0;
	int hVertexCount = 0;
	int channels = 0;
	int flags = 0;

	TextureLoader::LoadTextureGreyScale(filename, texData, wVertexCount, hVertexCount, channels, flags);
	OGLMesh* m = new OGLMesh();
	m->primType = Triangles;

	auto pixelPtr = &texData[0];
	int bytesPerPixel = 1;
	float maxPixelColor = 256.0f;
	for (int z = 0; z < hVertexCount; ++z) {
		for (int x = 0; x < wVertexCount; ++x) {

			float height = ((*pixelPtr)/ maxPixelColor) * heightMultiplier;
			m->positions.emplace_back(Vector3((float)x / ((float)wVertexCount - 1), height, (float)z / ((float)hVertexCount - 1)));
			m->texCoords.emplace_back(Vector2((float)x / ((float)wVertexCount - 1), (float)z / ((float)hVertexCount - 1)));
			pixelPtr += bytesPerPixel;
		}
	}

	stbi_image_free(texData);

	int i = 0;
	for (int z = 0; z < hVertexCount - 1; ++z) { 
		for (int x = 0; x < wVertexCount - 1; ++x) {
			int a = (z * (wVertexCount)) + x;
			int b = (z * (wVertexCount)) + (x + 1);
			int c = ((z + 1) * (wVertexCount)) + (x + 1);
			int d = ((z + 1) * (wVertexCount)) + x;
			m->indices.emplace_back(a);
			m->indices.emplace_back(c);
			m->indices.emplace_back(b);
			m->indices.emplace_back(c);
			m->indices.emplace_back(a);
			m->indices.emplace_back(d);
		}
	}

	m->AddSubMesh(0, (wVertexCount - 1) * (hVertexCount - 1) * 6, 0);
	m->CalculateNormals();
	m->CalculateTangents();

	m->UploadToGPU();
	return m;
}


OGLMesh* OGLMesh::GenerateQuad() {
	OGLMesh* m = new OGLMesh();
	m->primType = TriangleStrip;
	m->positions.emplace_back(Vector3(-1.0f, 1.0f, 0.0f));
	m->positions.emplace_back(Vector3(-1.0f, -1.0f, 0.0f));
	m->positions.emplace_back(Vector3(1.0f, 1.0f, 0.0f));
	m->positions.emplace_back(Vector3(1.0f, -1.0f, 0.0f));
	m->texCoords.emplace_back(Vector2(0.0f, 0.0f));
	m->texCoords.emplace_back(Vector2(0.0f, 1.0f));
	m->texCoords.emplace_back(Vector2(1.0f, 0.0f));
	m->texCoords.emplace_back(Vector2(1.0f, 1.0f));

	for (int i = 0; i < 4; ++i) {
		m->colours.emplace_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m->normals.emplace_back(Vector3(0.0f, 0.0f, -1.0f)); //New!
		m->tangents.emplace_back(Vector4(1.0f, 0.0f, 0.0f, 1.0f)); //New!
	}
	m->UploadToGPU();
	return m;
}

OGLMesh* NCL::Rendering::OGLMesh::GenerateHUDQuad() {
	OGLMesh* m = new OGLMesh();
	m->primType = GeometryPrimitive::TriangleStrip;
	m->positions.emplace_back(Vector3(-1.0f, 1.0f, 0.0f));
	m->positions.emplace_back(Vector3(-1.0f, -1.0f, 0.0f));
	m->positions.emplace_back(Vector3(1.0f, 1.0f, 0.0f));
	m->positions.emplace_back(Vector3(1.0f, -1.0f, 0.0f));
	
	m->UploadToGPU();
	return m;
}

OGLMesh::~OGLMesh()	{
	glDeleteVertexArrays(1, &vao);			//Delete our VAO
	glDeleteBuffers(VertexAttribute::MAX_ATTRIBUTES, attributeBuffers);	//Delete our VBOs
	glDeleteBuffers(1, &indexBuffer);	//Delete our indices
}

void CreateVertexBuffer(GLuint& buffer, int byteCount, void* data) {
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, byteCount, data, GL_STATIC_DRAW);
}

void OGLMesh::BindVertexAttribute(int attribSlot, int buffer, int bindingID, int elementCount, int elementSize, int elementOffset) {
	glEnableVertexAttribArray(attribSlot);
	glVertexAttribFormat(attribSlot, elementCount, GL_FLOAT, false, 0);
	glVertexAttribBinding(attribSlot, bindingID);

	glBindVertexBuffer(bindingID, buffer, elementOffset, elementSize);
}

void OGLMesh::BindVertexAttribute_i(int attribSlot, int buffer, int bindingID, int elementCount, int elementSize, int elementOffset) {
	glEnableVertexAttribArray(attribSlot);
	glVertexAttribIFormat(attribSlot, elementCount, GL_INT, 0);
	glVertexAttribBinding(attribSlot, bindingID);

	glBindVertexBuffer(bindingID, buffer, elementOffset, elementSize);
}

void OGLMesh::UploadToGPU(Rendering::RendererBase* renderer) {
	if (!ValidateMeshData()) {
		return;
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	int numVertices = GetVertexCount();
	int numIndices	= GetIndexCount();

	if (!GetPositionData().empty()) {
		CreateVertexBuffer(attributeBuffers[VertexAttribute::Positions], numVertices * sizeof(Vector3), (float*)GetPositionData().data());
		BindVertexAttribute(VertexAttribute::Positions, attributeBuffers[VertexAttribute::Positions], VertexAttribute::Positions, 3, sizeof(Vector3), 0);
	}

	if (!GetColourData().empty()) {	//buffer colour data
		CreateVertexBuffer(attributeBuffers[VertexAttribute::Colours], numVertices * sizeof(Vector4), (float*)GetColourData().data());
		BindVertexAttribute(VertexAttribute::Colours, attributeBuffers[VertexAttribute::Colours], VertexAttribute::Colours, 4, sizeof(Vector4), 0);
	}
	if (!GetTextureCoordData().empty()) {	//Buffer texture data
		CreateVertexBuffer(attributeBuffers[VertexAttribute::TextureCoords], numVertices * sizeof(Vector2), (float*)GetTextureCoordData().data());
		BindVertexAttribute(VertexAttribute::TextureCoords, attributeBuffers[VertexAttribute::TextureCoords], VertexAttribute::TextureCoords, 2, sizeof(Vector2), 0);
	}

	if (!GetNormalData().empty()) {	//Buffer normal data
		CreateVertexBuffer(attributeBuffers[VertexAttribute::Normals], numVertices * sizeof(Vector3), (float*)GetNormalData().data());
		BindVertexAttribute(VertexAttribute::Normals, attributeBuffers[VertexAttribute::Normals], VertexAttribute::Normals, 3, sizeof(Vector3), 0);
	}

	if (!GetTangentData().empty()) {	//Buffer tangent data
		CreateVertexBuffer(attributeBuffers[VertexAttribute::Tangents], numVertices * sizeof(Vector4), (float*)GetTangentData().data());
		BindVertexAttribute(VertexAttribute::Tangents, attributeBuffers[VertexAttribute::Tangents], VertexAttribute::Tangents, 4, sizeof(Vector4), 0);
	}

	if (!GetSkinWeightData().empty()) {	//Skeleton weights
		CreateVertexBuffer(attributeBuffers[VertexAttribute::JointWeights], numVertices * sizeof(Vector4), (float*)GetSkinWeightData().data());
		BindVertexAttribute(VertexAttribute::JointWeights, attributeBuffers[VertexAttribute::JointWeights], VertexAttribute::JointWeights, 4, sizeof(Vector4), 0);
	}

	if (!GetSkinIndexData().empty()) {	//Skeleton joint indices
		CreateVertexBuffer(attributeBuffers[VertexAttribute::JointIndices], numVertices * sizeof(Vector4i), (int*)GetSkinIndexData().data());
		BindVertexAttribute_i(VertexAttribute::JointIndices, attributeBuffers[VertexAttribute::JointIndices], VertexAttribute::JointIndices, 4, sizeof(Vector4i), 0);
	}

	if (!GetIndexData().empty()) {		//buffer index data
		glGenBuffers(1, &attributeBuffers[VertexAttribute::MAX_ATTRIBUTES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributeBuffers[VertexAttribute::MAX_ATTRIBUTES]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), (int*)GetIndexData().data(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OGLMesh::UpdateGPUBuffers(unsigned int startVertex, unsigned int vertexCount) {
	if (!GetPositionData().empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::Positions]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector3), vertexCount * sizeof(Vector3), (char*)&GetPositionData()[startVertex]);
	}

	if (!GetColourData().empty()) {	//buffer colour data
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::Colours]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4), vertexCount * sizeof(Vector4), (char*)&GetColourData()[startVertex]);
	}
	if (!GetTextureCoordData().empty()) {	//Buffer texture data
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::TextureCoords]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector2), vertexCount * sizeof(Vector2), (char*)&GetTextureCoordData()[startVertex]);
	}

	if (!GetNormalData().empty()) {	//Buffer normal data
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::Normals]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector3), vertexCount * sizeof(Vector3), (char*)&GetNormalData()[startVertex]);
	}

	if (!GetTangentData().empty()) {	//Buffer tangent data
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::Tangents]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4), vertexCount * sizeof(Vector4), (char*)&GetTangentData()[startVertex]);
	}

	if (!GetSkinWeightData().empty()) {	//Skeleton weights
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::JointWeights]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4), vertexCount * sizeof(Vector4), (char*)&GetSkinWeightData()[startVertex]);
	}

	if (!GetSkinIndexData().empty()) {	//Skeleton joint indices
		glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[VertexAttribute::JointIndices]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4i), vertexCount * sizeof(Vector4i), (char*)&GetSkinIndexData()[startVertex]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OGLMesh::RecalculateNormals() {
	normals.clear();

	if (indices.size() > 0) {
		for (size_t i = 0; i < positions.size(); i++) {
			normals.emplace_back(Vector3());
		}

		for (size_t i = 0; i < indices.size(); i += 3) {
			Vector3& a = positions[indices[i+0]];
			Vector3& b = positions[indices[i+1]];
			Vector3& c = positions[indices[i+2]];

			Vector3 normal = Vector3::Cross(b - a, c - a);
			normal.Normalise();

			normals[indices[i + 0]] += normal;
			normals[indices[i + 1]] += normal;
			normals[indices[i + 2]] += normal;
		}
		for (size_t i = 0; i < normals.size(); ++i) {
			normals[i].Normalise();
		}
	}
	else {

	}
}
#endif