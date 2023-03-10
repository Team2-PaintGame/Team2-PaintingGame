#pragma once
#ifdef _ORBIS
#include "MeshGeometry.h"
#include "PS4MemoryAware.h"
#include <gnm.h>
#include <gnmx\context.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\allocators.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\stack_allocator.h>
#include <gnm\dataformats.h>


namespace NCL::GNM {
	using namespace sce;
	class GNMMesh :
		public NCL::MeshGeometry, public PS4::PS4MemoryAware {
		//friend class GNMRenderer;
	public:
		GNMMesh();
		GNMMesh(const std::string& filename) {}
		~GNMMesh();
		
		static GNMMesh* GenerateTriangle();
		static GNMMesh* GenerateQuad();
		static GNMMesh* GenerateSinglePoint();
		static GNMMesh* GenerateFlatMesh(int hVertexCount = 128, int wVertexCount = 128) { return nullptr; }
		static GNMMesh* GenerateHeightMap(const std::string& filename, int heightMultiplier = 10) { return nullptr; }

		void	UploadToGPU(Rendering::RendererBase* renderer) override;
		void	SubmitDraw(Gnmx::GnmxGfxContext& cmdList, Gnm::ShaderStage stage);

		void	InitAttributeBuffer(sce::Gnm::Buffer& buffer, Gnm::DataFormat format, void* offset);

	protected:
		//Gpu simply has a 4 byte alignment!
		struct MeshVertex
		{
			float position[3];
			float textureCoord[2];
			float normal[3];
			float tangent[3];
		};

		sce::Gnm::IndexSize		indexType;
		sce::Gnm::PrimitiveType primitiveType;

		int* indexBuffer;
		MeshVertex* vertexBuffer;

		int	vertexDataSize;
		int indexDataSize;

		sce::Gnm::Buffer* attributeBuffers;
		int					attributeCount;
	};
}
#endif