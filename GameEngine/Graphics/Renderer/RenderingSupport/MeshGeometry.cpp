#include "MeshGeometry.h"
#include "Assets.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"


#include <fstream>
#include <string>

using namespace NCL;
using namespace Maths;

MeshGeometry::MeshGeometry()
{
	primType	= GeometryPrimitive::Triangles;
}

enum class GeometryChunkTypes {
	VPositions		= 1 << 0,		//1
	VNormals		= 1 << 1,		//2
	VTangents		= 1 << 2,		//4
	VColors			= 1 << 3,		//8
	VTex0			= 1 << 4,		//16
	VTex1			= 1 << 5,		//32
	VWeightValues	= 1 << 6,		//64
	VWeightIndices	= 1 << 7,		//128
	Indices			= 1 << 8,		//256
	JointNames		= 1 << 9,		//512
	JointParents	= 1 << 10,		//1024
	BindPose		= 1 << 11,		//2048
	BindPoseInv		= 1 << 12,		//4096
	Material		= 1 << 13,		//8192
	SubMeshes		= 1 << 14,		//16384
	SubMeshNames	= 1 << 15,
	BindPoseIndices = 1 << 16,
	BindPoseStates	= 1 << 17,
};

enum class GeometryChunkData {
	dFloat, //Just float data
	dShort, //Translate from -32k to 32k to a float
	dByte,	//Translate from -128 to 127 to a float
};

void* ReadVertexData(GeometryChunkData dataType, GeometryChunkTypes chunkType, int numVertices) {
	int numElements = 3;

	if (chunkType == GeometryChunkTypes::VTex0 ||
		chunkType == GeometryChunkTypes::VTex1) {
		numElements = 2;
	}
	else if (chunkType == GeometryChunkTypes::VColors) {
		numElements = 4;
	}

	int bytesPerElement = 4;

	if (dataType == GeometryChunkData::dShort) {
		bytesPerElement = 2;
	}
	else if (dataType == GeometryChunkData::dByte) {
		bytesPerElement = 1;
	}

	char* data = new char[numElements * bytesPerElement];

	return data;
}

void ReadTextInts(std::ifstream& file, vector<Vector2i>& element, int numVertices) {
	element.reserve(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		Vector2i temp;
		file >> temp.array[0];
		file >> temp.array[1];
		element.emplace_back(temp);
	}
}

void ReadTeReadTextIntsxtFloats(std::ifstream& file, vector<Vector3i>& element, int numVertices) {
	element.reserve(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		Vector3i temp;
		file >> temp.array[0];
		file >> temp.array[1];
		file >> temp.array[2];
		element.emplace_back(temp);
	}
}

void ReadTextInts(std::ifstream& file, vector<Vector4i>& element, int numVertices) {
	element.reserve(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		Vector4i temp;
		file >> temp.array[0];
		file >> temp.array[1];
		file >> temp.array[2];
		file >> temp.array[3];
		element.emplace_back(temp);
	}
}

void ReadTextFloats(std::ifstream& file, vector<Vector2>& element, int numVertices) {
	element.reserve(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		Vector2 temp;
		file >> temp.x;
		file >> temp.y;
		element.emplace_back(temp);
	}
}

void ReadTextFloats(std::ifstream& file, vector<Vector3>& element, int numVertices) {
	element.reserve(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		Vector3 temp;
		file >> temp.x;
		file >> temp.y;
		file >> temp.z;
		element.emplace_back(temp);
	}
}

void ReadTextFloats(std::ifstream& file, vector<Vector4>& element, int numVertices) {
	element.reserve(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		Vector4 temp;
		file >> temp.x;
		file >> temp.y;
		file >> temp.z;
		file >> temp.w;
		element.emplace_back(temp);
	}
}

void ReadIndices(std::ifstream& file, vector<unsigned int>& elements, int numIndices) {
	elements.reserve(numIndices);
	for (int i = 0; i < numIndices; ++i) {
		unsigned int temp;
		file >> temp;
		elements.emplace_back(temp);
	}
}

void ReadIntegerArray(std::ifstream& file, vector<int>& into) {//New!
	int count = 0;
	file >> count;
	into.reserve(count);
	for (int i = 0; i < count; ++i) {
		int r = 0;
		file >> r;
		into.push_back(r);
	}
}

void ReadBindposes(std::ifstream& file, vector<SubMeshPoses>& bindPoses) {//New!
	int poseCount = 0;
	file >> poseCount;
	bindPoses.reserve(poseCount);
	for (int i = 0; i < poseCount; ++i) {
		SubMeshPoses m;
		file >> m.start;
		file >> m.count;
		bindPoses.emplace_back(m);
	}
}

MeshGeometry::MeshGeometry(const std::string&filename) {
	primType = GeometryPrimitive::Triangles;
	std::ifstream file(Assets::MESHDIR + filename);

	std::string filetype;
	int fileVersion;

	file >> filetype;

	if (filetype != "MeshGeometry") {
		std::cout << __FUNCTION__ << " File is not a MeshGeometry file!\n";
		return;
	}

	file >> fileVersion;

	if (fileVersion != 1) {
		std::cout << __FUNCTION__ << " MeshGeometry file has incompatible version!\n";
		return;
	}

	int numMeshes	= 0; //read
	int numVertices = 0; //read
	int numIndices	= 0; //read
	int numChunks   = 0; //read

	file >> numMeshes;
	file >> numVertices;
	file >> numIndices;
	file >> numChunks;
	
	for (int i = 0; i < numChunks; ++i) {
		int chunkType = (int)GeometryChunkTypes::VPositions;

		file >> chunkType;

		switch ((GeometryChunkTypes)chunkType) {
			case GeometryChunkTypes::VPositions:ReadTextFloats(file, positions, numVertices);	break;
			case GeometryChunkTypes::VColors:	ReadTextFloats(file, colours, numVertices);		break;
			case GeometryChunkTypes::VNormals:	ReadTextFloats(file, normals, numVertices);		break;
			case GeometryChunkTypes::VTangents:	ReadTextFloats(file, tangents, numVertices);	break;
			case GeometryChunkTypes::VTex0:		ReadTextFloats(file, texCoords, numVertices);	break;
			case GeometryChunkTypes::Indices:	ReadIndices(file, indices, numIndices); break;			
				
			case GeometryChunkTypes::VWeightValues:		ReadTextFloats(file, skinWeights, numVertices);  break;
			case GeometryChunkTypes::VWeightIndices:	ReadTextInts(file, skinIndices, numVertices);  break;
			case GeometryChunkTypes::JointNames:		ReadJointNames(file);		break;
			case GeometryChunkTypes::JointParents:		ReadJointParents(file);		break;
			case GeometryChunkTypes::BindPose:			ReadRigPose(file, bindPose);  break;
			case GeometryChunkTypes::BindPoseInv:		ReadRigPose(file, inverseBindPose);  break;
			case GeometryChunkTypes::SubMeshes: 		ReadSubMeshes(file, numMeshes); break;
			case GeometryChunkTypes::SubMeshNames: 		ReadSubMeshNames(file, numMeshes); break;

			case GeometryChunkTypes::BindPoseIndices:	ReadIntegerArray(file, bindPoseIndices); break;//New!
			case GeometryChunkTypes::BindPoseStates:	ReadBindposes(file, bindPoseStates); break;//New!

		}
	}
}

MeshGeometry::~MeshGeometry()
{
}

bool MeshGeometry::HasTriangle(unsigned int i) const {
	int triCount = 0;
	if (GetIndexCount() > 0) {
		triCount = GetIndexCount() / 3;
	}
	else {
		triCount = GetVertexCount() / 3;
	}
	return i < (unsigned int)triCount;
}


bool	MeshGeometry::GetVertexIndicesForTri(unsigned int i, unsigned int& a, unsigned int& b, unsigned int& c) const {
	if (!HasTriangle(i)) {
		return false;
	}
	if (GetIndexCount() > 0) {
		a = indices[(i * 3)];
		b = indices[(i * 3) + 1];
		c = indices[(i * 3) + 2];
	}
	else {
		a = (i * 3);
		b = (i * 3) + 1;
		c = (i * 3) + 2;
	}
	return true;
}

bool MeshGeometry::GetTriangle(unsigned int i, Vector3& va, Vector3& vb, Vector3& vc) const {
	bool hasTri = false;
	unsigned int a, b, c;
	hasTri = GetVertexIndicesForTri(i, a, b, c);
	if (!hasTri) {
		return false;
	}
	va = positions[a];
	vb = positions[b];
	vc = positions[c];
	return true;
}


bool MeshGeometry::GetNormalForTri(unsigned int i, Vector3& n) const {
	Vector3 a, b, c;

	bool hasTri = GetTriangle(i, a, b, c);
	if (!hasTri) {
		return false;
	}

	Vector3 ba = b - a;
	Vector3 ca = c - a;
	n = Vector3::Cross(ba, ca);
	n.Normalise();
	return true;
}

bool MeshGeometry::GetTangentForTri(unsigned int i, Vector4& t) const {
	Vector3 a, b, c;

	bool hasTri = GetTriangle(i, a, b, c);
	if (!hasTri) {
		return false;
	}

	Vector3 ba = b - a;
	Vector3 ca = c - a;

	unsigned int ai, bi, ci;
	GetVertexIndicesForTri(i, ai, bi, ci);
	Vector2 tba = texCoords[bi] - texCoords[ai];
	Vector2 tca = texCoords[ci] - texCoords[ai];
	
	Matrix2 texMatrix = Matrix2(tba, tca);
	texMatrix.Invert();
	Vector3 tangent;
	Vector3 binormal;
	tangent = ba * texMatrix.array[0][0] + ca * texMatrix.array[0][1];
	binormal = ba * texMatrix.array[1][0] + ca * texMatrix.array[1][1];
	Vector3 normal = Vector3::Cross(ba, ca);
	Vector3 biCross = Vector3::Cross(tangent, normal);
	float handedness = 1.0f;
	if (Vector3::Dot(biCross, binormal) < 0.0f) {
		handedness = -1.0f;
	}

	t.x = tangent.x;
	t.y = tangent.y;
	t.z = tangent.z;
	t.w = handedness;

	float handedness_ = t.w > 0.0f ? 1.0f : -1.0f;
	t.w = 0.0f;
	t.Normalise();
	t.w = handedness_;

}

Vector4 MeshGeometry::GetTangent(int a, int b, int c) {
	Vector3 ba = positions[b] - positions[a];
	Vector3 ca = positions[c] - positions[a];
	Vector2 tba = texCoords[b] - texCoords[a];
	Vector2 tca = texCoords[c] - texCoords[a];
	Matrix2 texMatrix = Matrix2(tba, tca);
	texMatrix.Invert();
	Vector3 tangent;
	Vector3 binormal;
	tangent = ba * texMatrix.array[0][0] + ca * texMatrix.array[0][1];
	binormal = ba * texMatrix.array[1][0] + ca * texMatrix.array[1][1];
	Vector3 normal = Vector3::Cross(ba, ca);
	Vector3 biCross = Vector3::Cross(tangent, normal);
	float handedness = 1.0f;
	if (Vector3::Dot(biCross, binormal) < 0.0f) {
		handedness = -1.0f;
	}

	Vector4 t(tangent.x, tangent.y, tangent.z, handedness);

	float handedness_ = t.w > 0.0f ? 1.0f : -1.0f;
	t.w = 0.0f;
	t.Normalise();
	t.w = handedness_;
	return t;
}

void	MeshGeometry::TransformVertices(const Matrix4& byMatrix) {

}

void MeshGeometry::CalculateNormals() {
	int triCount = GetTriCount();

	for (size_t i = 0; i < positions.size(); i++) {
		normals.emplace_back(Vector3());
	}

	for (int i = 0; i < triCount; ++i) {
		unsigned int a = 0;
		unsigned int b = 0;
		unsigned int c = 0;
		GetVertexIndicesForTri(i, a, b, c);
		Vector3 normal = Vector3::Cross((positions[b] - positions[a]), (positions[c] - positions[a]));
		normal.Normalise();
		normals[a] += normal;
		normals[b] += normal;
		normals[c] += normal;
	}
}

void MeshGeometry::CalculateTangents() {
	int triCount = GetTriCount();

	for (size_t i = 0; i < positions.size(); i++) {
		tangents.emplace_back(Vector4());
	}

	for (int i = 0; i < triCount; ++i) {
		unsigned int a = 0;
		unsigned int b = 0;
		unsigned int c = 0;
		GetVertexIndicesForTri(i, a, b, c);
		Vector4 tangent = GetTangent(a, b, c);
		tangents[a] += tangent;
		tangents[b] += tangent;
		tangents[c] += tangent;
	}
}

void	MeshGeometry::RecalculateNormals() {

}

void	MeshGeometry::RecalculateTangents() {

}

void MeshGeometry::SetVertexPositions(const vector<Vector3>& newVerts) {
	positions = newVerts;
}

void MeshGeometry::SetVertexTextureCoords(const vector<Vector2>& newTex) {
	texCoords = newTex;
}

void MeshGeometry::SetVertexColours(const vector<Vector4>& newColours) {
	colours = newColours;
}

void MeshGeometry::SetVertexNormals(const vector<Vector3>& newNorms) {
	normals = newNorms;
}

void MeshGeometry::SetVertexTangents(const vector<Vector4>& newTans) {
	tangents = newTans;
}

void MeshGeometry::SetVertexIndices(const vector<unsigned int>& newIndices) {
	indices = newIndices;
}

void MeshGeometry::SetVertexSkinWeights(const vector<Vector4>& newSkinWeights) {
	skinWeights = newSkinWeights;
}

void MeshGeometry::SetVertexSkinIndices(const vector<Vector4i>& newSkinIndices) {
	skinIndices = newSkinIndices;
}

MeshGeometry* MeshGeometry::GenerateTriangle(MeshGeometry* input) {
	input->SetVertexPositions({Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(0,1,0) });
	input->SetVertexColours({ Vector4(1,0,0,1), Vector4(0,1,0,1), Vector4(0,0,1,1) });
	input->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(0.5, 1) });
	input->SetVertexIndices({ 0,1,2 });
	input->debugName = "Test-Triangle";
	return input;
}

void MeshGeometry::SetDebugName(const std::string& newName) {
	debugName = newName;
}

int MeshGeometry::GetIndexForJoint(const std::string& name) const {
	for (int i = 0; i < jointNames.size(); ++i) {
		if (jointNames[i] == name) {
			return i;
		}
	}
	return -1;
}

void MeshGeometry::SetJointNames(std::vector < std::string >& newNames) {
	jointNames = newNames;
}

void MeshGeometry::SetJointParents(std::vector<int>& newParents) {
	jointParents = newParents;
}

void MeshGeometry::SetBindPose(std::vector<Matrix4>& newMats) {
	bindPose = newMats;
}

void MeshGeometry::SetInverseBindPose(std::vector<Matrix4>& newMats) {
	inverseBindPose = newMats;
}

void MeshGeometry::CalculateInverseBindPose() {
	inverseBindPose.resize(bindPose.size());

	for (int i = 0; i < bindPose.size(); ++i) {
		inverseBindPose[i] = bindPose[i].Inverse();
	}
}

void MeshGeometry::ReadRigPose(std::ifstream& file, vector<Matrix4>& into) {
	int matCount = 0;
	file >> matCount;

	for (int m = 0; m < matCount; ++m) {
		Matrix4 mat;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				file >> mat.array[i][j];
			}
		}

		into.emplace_back(mat);
	}
}

void MeshGeometry::ReadJointParents(std::ifstream& file) {
	int jointCount = 0;
	file >> jointCount;

	for (int i = 0; i < jointCount; ++i) {
		int id = -1;
		file >> id;
		jointParents.emplace_back(id);
	}
}

void MeshGeometry::ReadJointNames(std::ifstream& file) {
	int jointCount = 0;
	file >> jointCount;
	std::string jointName;
	std::getline(file, jointName);

	for (int i = 0; i < jointCount; ++i) {
		std::string jointName;
		std::getline(file, jointName);
		jointNames.emplace_back(jointName);
	}
}

void MeshGeometry::ReadSubMeshes(std::ifstream& file, int count) {
	for (int i = 0; i < count; ++i) {
		SubMesh m;
		file >> m.start;
		file >> m.count;
		subMeshes.emplace_back(m);
	}
}

void MeshGeometry::ReadSubMeshNames(std::ifstream& file, int count) {
	std::string scrap;
	std::getline(file, scrap);

	for (int i = 0; i < count; ++i) {
		std::string meshName;
		std::getline(file, meshName);
		subMeshNames.emplace_back(meshName);
	}
}

bool MeshGeometry::ValidateMeshData() {
	if (GetPositionData().empty()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " does not have any vertex positions!\n";
		return false;
	}
	if (!GetTextureCoordData().empty() && GetTextureCoordData().size() != GetVertexCount()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " has an incorrect texture coordinate attribute count!\n";
		return false;
	}
	if (!GetColourData().empty() && GetColourData().size() != GetVertexCount()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " has an incorrect colour attribute count!\n";
		return false;
	}
	if (!GetNormalData().empty() && GetNormalData().size() != GetVertexCount()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " has an incorrect normal attribute count!\n";
		return false;
	}
	if (!GetTangentData().empty() && GetTangentData().size() != GetVertexCount()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " has an incorrect tangent attribute count!\n";
		return false;
	}

	if (!GetSkinWeightData().empty() && GetSkinWeightData().size() != GetVertexCount()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " has an incorrect skin weight attribute count!\n";
		return false;
	}

	if (!GetSkinIndexData().empty() && GetSkinIndexData().size() != GetVertexCount()) {
		std::cout << __FUNCTION__ << " mesh " << debugName << " has an incorrect skin index attribute count!\n";
		return false;
	}
	return true;
}