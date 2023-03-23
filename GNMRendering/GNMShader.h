#pragma once
#ifdef __ORBIS__
#include "ShaderBase.h"
#include "PS4MemoryAware.h"

#include <gnm.h>
#include <gnmx\shaderbinary.h>
#include <gnmx\context.h>

#include <..\samples\sample_code\graphics\api_gnm\toolkit\allocators.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\stack_allocator.h>
#include <vector>
#include <shader\binary.h>

using std::vector;
using namespace sce;
using namespace NCL::PS4;

namespace NCL::GNM {
	class GNMShader :
		public Rendering::ShaderBase, public PS4MemoryAware {
		friend class PS4Renderer;
	public:
		~GNMShader();

		static GNMShader* GenerateShader(const string& vertex, const string& pixel);

		int		GetConstantBufferIndex(const string& name, const sce::Gnm::ShaderStage& shaderStage = Gnm::kShaderStageVs);
		void	SubmitShaderSwitch(Gnmx::GnmxGfxContext& cmdList);

	protected:
		GNMShader();

		void ReloadShader() override {
			//we load binaries in directly...
		}

		void GenerateVertexShader(const string& name, bool makeFetch);

		void GeneratePixelShader(const string& name);

		void GenerateFetchShader(char* binData);

		bool LoadShaderText(const string& name, string& into);

		bool LoadShaderBinary(const string& name, char*& into, int& dataSize);

		bool ShaderIsBinary(const string& name);

	protected:
		void* fetchShader;

		sce::Shader::Binary::Program	vertexBinary;
		Gnmx::VsShader* vertexShader;
		Gnmx::InputOffsetsCache			vertexCache;

		sce::Shader::Binary::Program	pixelBinary;
		Gnmx::PsShader* pixelShader;
		Gnmx::InputOffsetsCache			pixelCache;
	};
}
#endif