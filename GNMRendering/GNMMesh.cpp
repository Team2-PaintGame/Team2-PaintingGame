#ifdef __ORBIS__
#include "GNMMesh.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace NCL::Maths;
using namespace NCL::GNM;

GNMMesh::GNMMesh()	{
	indexBuffer		= 0;
	vertexBuffer	= 0;
	attributeCount	= 0;
}

GNMMesh::GNMMesh(const std::string& filename) : MeshGeometry(filename) {
	indexType = sce::Gnm::IndexSize::kIndexSize32;
	primitiveType = sce::Gnm::PrimitiveType::kPrimitiveTypeTriList;
}

GNMMesh::~GNMMesh()	{
	delete[] attributeBuffers;
}

GNMMesh* GNMMesh::GenerateQuad() {
	GNMMesh* mesh = new GNMMesh();

	mesh->indexType		= sce::Gnm::IndexSize::kIndexSize32;
	mesh->primitiveType = sce::Gnm::PrimitiveType::kPrimitiveTypeTriStrip;

	mesh->SetVertexPositions({ Vector3(-1.0f, 1.0f, 0.0f), Vector3(-1.0f, -1.0f, 0.0f),Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f, -1.0f, 0.0f) });

	std::vector<Vector3> normals;
	std::vector<Vector4> tangents;
	std::vector<unsigned int> indices;

	for (int i = 0; i < 4; ++i) {
		normals.emplace_back(Vector3(0, 0, 1));
		tangents.emplace_back(Vector4(1, 0, 0, 0));

		indices.emplace_back(i);
	}
	mesh->SetVertexTextureCoords({ Vector2(0.0f, 0.0f) , Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f)});
	mesh->SetVertexNormals(normals);
	mesh->SetVertexTangents(tangents);
	mesh->SetVertexIndices(indices);

	mesh->UploadToGPU();

	return mesh;
}

GNMMesh* GNMMesh::GenerateSinglePoint() {
	GNMMesh* mesh = new GNMMesh();

	mesh->indexType		= sce::Gnm::IndexSize::kIndexSize32;
	mesh->primitiveType = sce::Gnm::PrimitiveType::kPrimitiveTypePointList;

	mesh->SetVertexPositions({ Vector3(0.0f, 0.0f, 0.0f) });
	mesh->SetVertexNormals({ Vector3(0, 0, 1) });
	mesh->SetVertexTangents({ Vector3(1, 0, 0) });
	mesh->SetVertexIndices({ 0 });

	mesh->UploadToGPU();

	return mesh;
}

GNMMesh* GNMMesh::GenerateTriangle() {
	GNMMesh* mesh = new GNMMesh();

	mesh->indexType		= sce::Gnm::IndexSize::kIndexSize32;
	mesh->primitiveType = sce::Gnm::PrimitiveType::kPrimitiveTypeTriList;

	mesh->SetVertexPositions({ Vector3(0.0f, 0.5f, 0.0f), Vector3(0.5f, -0.5f, 0.0f), Vector3(-0.5f, -0.5f, 0.0f) });
	mesh->SetVertexTextureCoords({ Vector2(0.5f, 0.0f) , Vector2(1.0f, 1.0f), Vector2(0.0f, 1.0f) });
	mesh->SetVertexNormals({ Vector3(0, 0, 1),Vector3(0, 0, 1), Vector3(0, 0, 1) });
	mesh->SetVertexTangents({ Vector4(1, 0, 0, 0), Vector4(1, 0, 0,0), Vector4(1, 0, 0,0) });
	mesh->SetVertexIndices({ 0, 1, 2 });

	mesh->UploadToGPU();

	return mesh;
}

void	GNMMesh::UploadToGPU(Rendering::RendererBase* renderer) {
	vertexDataSize = GetVertexCount() * sizeof(MeshVertex);
	indexDataSize  = GetIndexCount() * sizeof(int);

	indexBuffer = static_cast<int*>			(garlicAllocator->allocate(indexDataSize, Gnm::kAlignmentOfBufferInBytes));
	vertexBuffer = static_cast<MeshVertex*>	(garlicAllocator->allocate(vertexDataSize, Gnm::kAlignmentOfBufferInBytes));

	Gnm::registerResource(nullptr, ownerHandle, indexBuffer , indexDataSize , "IndexData" , Gnm::kResourceTypeIndexBufferBaseAddress, 0);
	Gnm::registerResource(nullptr, ownerHandle, vertexBuffer, vertexDataSize, "VertexData", Gnm::kResourceTypeIndexBufferBaseAddress, 0);

	/*for (int i = 0; i < GetVertexCount(); ++i) {
		memcpy(&vertexBuffer[i].position,		&positions[i], sizeof(float) * 3);
		memcpy(&vertexBuffer[i].textureCoord,	&texCoords[i], sizeof(float) * 2);
		memcpy(&vertexBuffer[i].colour,			&colours[i], sizeof(float) * 4);
		memcpy(&vertexBuffer[i].normal,			&normals[i],   sizeof(float) * 3);
		memcpy(&vertexBuffer[i].tangent,		&tangents[i],  sizeof(float) * 3);
		memcpy(&vertexBuffer[i].skinWeight,		&skinWeights[i], sizeof(float) * 4);
		memcpy(&vertexBuffer[i].skinIndex,		&skinIndices[i], sizeof(int) * 4);
	}*/

	for (int i = 0; i < GetIndexCount(); ++i) { //Our index buffer might not have the same data size as the source indices?
		indexBuffer[i] = indices[i];
	}	

	attributeCount		= 7;
	attributeBuffers	= new sce::Gnm::Buffer[7];

	if (!GetPositionData().empty()) {
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].position, &positions[i], sizeof(float) * 3);
		}
		InitAttributeBuffer(attributeBuffers[0], Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].position));
	}

	if (!GetTextureCoordData().empty()) {	//Buffer texture data
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].textureCoord, &texCoords[i], sizeof(float) * 2);
		}
		InitAttributeBuffer(attributeBuffers[1], Gnm::kDataFormatR32G32Float, &(vertexBuffer[0].textureCoord));
	}

	if (!GetColourData().empty()) {	//buffer colour data
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].colour, &colours[i], sizeof(float) * 4);
		}
		InitAttributeBuffer(attributeBuffers[2], Gnm::kDataFormatR32G32Float, &(vertexBuffer[0].colour));
	}

	if (!GetNormalData().empty()) {	//Buffer normal data
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].normal, &normals[i], sizeof(float) * 3);
		}
		InitAttributeBuffer(attributeBuffers[2], Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].normal));
	}

	if (!GetTangentData().empty()) {	//Buffer tangent data
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].tangent, &tangents[i], sizeof(float) * 3);
		}
		InitAttributeBuffer(attributeBuffers[3], Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].tangent));
	}

	if (!GetSkinWeightData().empty()) {	//Skeleton weights
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].skinWeight, &skinWeights[i], sizeof(float) * 4);
		}
		InitAttributeBuffer(attributeBuffers[4], Gnm::kDataFormatR32G32B32Float, &(vertexBuffer[0].skinWeight));
	}

	if (!GetSkinIndexData().empty()) {	//Skeleton joint indices
		for (int i = 0; i < GetVertexCount(); ++i) {
			memcpy(&vertexBuffer[i].skinIndex, &skinIndices[i], sizeof(int) * 4);
		}
		InitAttributeBuffer(attributeBuffers[5], Gnm::kDataFormatR32G32B32A32Sint, &(vertexBuffer[0].skinIndex));
	}
}

void	GNMMesh::InitAttributeBuffer(sce::Gnm::Buffer &buffer, Gnm::DataFormat format, void*offset) {
	buffer.initAsVertexBuffer(offset, format, sizeof(MeshVertex), GetVertexCount());
	buffer.setResourceMemoryType(Gnm::kResourceMemoryTypeRO);
}

void GNMMesh::SubmitDraw(Gnmx::GnmxGfxContext& cmdList, Gnm::ShaderStage stage, int numInstances) {
	cmdList.setVertexBuffers(stage, 0, attributeCount, attributeBuffers);
	cmdList.setPrimitiveType(primitiveType);
	cmdList.setIndexSize(indexType);
	if (numInstances > 0) {
		cmdList.drawIndexMultiInstanced(GetIndexCount(), numInstances, indexBuffer, attributeBuffers);
	}
	else {
		cmdList.drawIndex(GetIndexCount(), indexBuffer);
	}
} 
#endif