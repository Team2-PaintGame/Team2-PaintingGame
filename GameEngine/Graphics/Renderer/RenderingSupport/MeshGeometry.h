/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <vector>
#include <string>

#include "Vector3.h"
#include "Matrix2.h"
#include "Maths.h"

using std::vector;

namespace NCL {
	namespace Maths {
		class Vector2;
		class Vector3;
		class Vector4;
		class Matrix4;
		class Vector4i;
	}
	namespace Rendering {
		class RendererBase;
	}
	using namespace Maths;

	enum GeometryPrimitive {
		Points,
		Lines,
		Triangles,
		TriangleFan,
		TriangleStrip,
		Patches
	};

	enum VertexAttribute {
		Positions,
		Colours,
		TextureCoords,
		Normals,
		Tangents,
		JointWeights,
		JointIndices,
		MAX_ATTRIBUTES
	};

	const std::string VertexAttributeNames[] = {
		std::string("Positions"),
		std::string("Colours"),
		std::string("Tex Coords"),
		std::string("Normals"),
		std::string("Tangents"),
		std::string("Joint Weights"),
		std::string("Joint Indices"),
	};

	struct SubMesh {
		int start = 0;
		int count = 0;
		int base  = 0;
	};

	struct SubMeshPoses { //New!
		int start;
		int count;
	};

	class MeshGeometry	{
	public:		
		virtual ~MeshGeometry();

		GeometryPrimitive GetPrimitiveType() const {
			return primType;
		}

		void SetPrimitiveType(GeometryPrimitive type) {
			primType = type;
		}

		unsigned int GetVertexCount() const {
			return (unsigned int)positions.size();
		}

		unsigned int GetIndexCount()  const {
			return (unsigned int)indices.size();
		}

		unsigned int GetJointCount() const {
			return (unsigned int)jointNames.size();
		}

		unsigned int GetSubMeshCount() const {
			return (unsigned int)subMeshes.size();
		}

		const SubMesh* GetSubMesh(unsigned int i) const {
			if (i > subMeshes.size()) {
				return nullptr;
			}
			return &subMeshes[i];
		}

		void AddSubMesh(int startIndex, int indexCount, int baseVertex) {
			SubMesh m;
			m.base = baseVertex;
			m.count = indexCount;
			m.start = startIndex;

			subMeshes.push_back(m);
		}

		int GetIndexForJoint(const std::string &name) const;

		const vector<Matrix4>& GetBindPose() const {
			return bindPose;
		}
		const vector<Matrix4>& GetInverseBindPose() const {
			return inverseBindPose;
		}

		void SetJointNames(std::vector < std::string > & newnames);
		void SetJointParents(std::vector<int>& newParents);
		void SetBindPose(std::vector<Matrix4>& newMats);
		void SetInverseBindPose(std::vector<Matrix4>& newMats);
		void CalculateInverseBindPose();



		bool GetTriangle(unsigned int i, Vector3& a, Vector3& b, Vector3& c) const;
		bool GetNormalForTri(unsigned int i, Vector3& n) const;
		bool GetTangentForTri(unsigned int i, Vector4& t) const;
		Vector4 GetTangent(int a, int b, int c);
		bool HasTriangle(unsigned int i) const;

		const vector<Vector3>&		GetPositionData()		const { return positions;	}
		const vector<Vector2>&		GetTextureCoordData()	const { return texCoords;	}
		const vector<Vector4>&		GetColourData()			const { return colours;		}
		const vector<Vector3>&		GetNormalData()			const { return normals;		}
		const vector<Vector4>&		GetTangentData()		const { return tangents;	}


		const vector<Vector4>& GetSkinWeightData()		const { return skinWeights; }
		const vector<Vector4i>& GetSkinIndexData()		const { return skinIndices; }

		const vector<int>& GetJointParents()	const {
			return jointParents;
		}


		const vector<unsigned int>& GetIndexData()			const { return indices;		}


		void SetVertexPositions(const vector<Vector3>& newVerts);
		void SetVertexTextureCoords(const vector<Vector2>& newTex);

		void SetVertexColours(const vector<Vector4>& newColours);
		void SetVertexNormals(const vector<Vector3>& newNorms);
		void SetVertexTangents(const vector<Vector4>& newTans);
		void SetVertexIndices(const vector<unsigned int>& newIndices);

		void SetVertexSkinWeights(const vector<Vector4>& newSkinWeights);
		void SetVertexSkinIndices(const vector<Vector4i>& newSkinIndices);


		void	TransformVertices(const Matrix4& byMatrix);

		void CalculateNormals();
		void CalculateTangents();
		void RecalculateNormals();
		void RecalculateTangents();

		void SetDebugName(const std::string& debugName);

		virtual void UploadToGPU(Rendering::RendererBase* renderer = nullptr) = 0;
		static MeshGeometry* GenerateTriangle(MeshGeometry* input);

	protected:
		MeshGeometry();
		MeshGeometry(const std::string&filename);

		void ReadRigPose(std::ifstream& file, vector<Matrix4>& into);
		void ReadJointParents(std::ifstream& file);
		void ReadJointNames(std::ifstream& file);
		void ReadSubMeshes(std::ifstream& file, int count);
		void ReadSubMeshNames(std::ifstream& file, int count);

		bool	GetVertexIndicesForTri(unsigned int i, unsigned int& a, unsigned int& b, unsigned int& c) const;

		virtual bool ValidateMeshData();
		unsigned int GetTriCount() const {
			int primCount = indices.size() ? indices.size() : positions.size();
			return primCount / 3;
		}

		std::string			debugName; //used when an API allows setting debug tags
		GeometryPrimitive	primType;
		vector<Vector3>		positions;

		vector<Vector2>			texCoords;
		vector<Vector4>			colours;
		vector<Vector3>			normals;
		vector<Vector4>			tangents;
		vector<unsigned int>	indices;

		vector<SubMesh>			subMeshes;
		vector<std::string>		subMeshNames;

		//Allows us to have 4 weight skinning 
		vector<Vector4>		skinWeights;
		vector<Vector4i>	skinIndices;
		vector<std::string>	jointNames;
		vector<int>			jointParents;

		std::vector<Matrix4>		bindPose;
		std::vector<Matrix4>		inverseBindPose;

		std::vector<int>			bindPoseIndices; //New!
		std::vector<SubMeshPoses>	bindPoseStates;  //New!
	};
}