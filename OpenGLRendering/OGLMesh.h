/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#ifdef _WIN32
#include "MeshGeometry.h"
#include "glad\gl.h"
#include <string>

namespace NCL {
	namespace Rendering {
		class OGLMesh : public NCL::MeshGeometry
		{
		public:
			friend class OGLRenderer;
			OGLMesh();
			OGLMesh(const std::string&filename);
			static OGLMesh* GenerateFlatMesh(int hVertexCount = 128, int wVertexCount = 128);
			static OGLMesh* GenerateHeightMap(const std::string& filename, int heightMultiplier = 10);

			static OGLMesh* GenerateQuad();
			static OGLMesh* GenerateHUDQuad();

			~OGLMesh();

			void RecalculateNormals();

			void UploadToGPU(Rendering::RendererBase* renderer = nullptr) override;
			void UpdateGPUBuffers(unsigned int startVertex, unsigned int vertexCount);
		protected:
			GLuint	GetVAO()			const { return vao;			}
			void BindVertexAttribute(int attribSlot, int bufferID, int bindingID, int elementCount, int elementSize, int elementOffset);
			void BindVertexAttribute_i(int attribSlot, int bufferID, int bindingID, int elementCount, int elementSize, int elementOffset);

			int		subCount;

			GLuint vao;
			GLuint oglType;
			GLuint attributeBuffers[VertexAttribute::MAX_ATTRIBUTES];
			GLuint indexBuffer;
		};
	}
}
#endif