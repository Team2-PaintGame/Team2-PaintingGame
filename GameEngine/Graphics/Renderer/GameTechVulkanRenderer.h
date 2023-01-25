#pragma once
#ifdef USEVULKAN
#include "VulkanRenderer.h"
#include "VulkanMesh.h"
#include "GameWorld.h"

namespace NCL::Rendering {
	class TextureBase;
	class ShaderBase;
	class GameTechVulkanRenderer : public VulkanRenderer
	{
	public:
		GameTechVulkanRenderer(GameWorld& world);
		~GameTechVulkanRenderer();

		MeshGeometry*	LoadMesh(const string& name);
		TextureBase*	LoadTexture(const string& name);
		ShaderBase*		LoadShader(const string& vertex, const string& fragment);

	protected:
		struct GlobalData {
			Matrix4 shadowMatrix;
			Matrix4 viewProjMatrix;
			Matrix4 viewMatrix;
			Matrix4 projMatrix;
			Matrix4 orthoMatrix;
			Vector3	lightPosition;
			float	lightRadius;
			Vector4	lightColour;
			Vector3	cameraPos;
		};

		struct ObjectState {
			Matrix4 modelMatrix;
			Vector4 colour;
			int		index[4];
		};

		struct ObjectData {
			VulkanTexture* cachedTex;
			int index;
		};

		class VulkanGameTechTexture : public NCL::Rendering::VulkanTexture
		{
			friend class GameTechVulkanRenderer;
		public:
			VulkanGameTechTexture(const std::string& filename) : VulkanTexture(filename) {
				index = 0;
			}
		protected:
			int index;
		};

		struct FrameData {
			vk::UniqueFence fence;

			VulkanBuffer	dataBuffer;
			char*			dataStart;	//Start of our buffer
			char*			data;		//We're bumping this along as we write data

			int globalDataOffset	= 0;	//Where does the global data start in the buffer?
			int objectStateOffset	= 0;	//Where does the object states start?
			int debugLinesOffset	= 0;	//Where do the debug lines start?
			int debugTextOffset		= 0;	//Where do the debug text verts start?

			size_t			objectCount;
			size_t			debugVertSize;
			size_t			bytesWritten;

			size_t lineVertCount = 0;
			size_t textVertCount = 0;

			size_t lineVertSize = 0;
			size_t textVertSize = 0;

			vk::UniqueDescriptorSet dataDescriptor;

			template<typename T>
			void WriteData(T value) {
				memcpy(data, &value, sizeof(T));
				data += sizeof(T);
				bytesWritten += sizeof(T);
			}
			void WriteData(void* inData, size_t byteCount) {
				memcpy(data, inData, byteCount);
				data += byteCount;
				bytesWritten += byteCount;
			}

			void AlignData(size_t alignment) {
				char* oldData = data;
				//data = (char*)(((uintptr_t)data + alignment - 1) & ~(uintptr_t)alignment);

				data = (char*)((((uintptr_t)data + alignment - 1) / (uintptr_t)alignment) * (uintptr_t)alignment);
				bytesWritten += data - oldData;
			}
		};

		FrameData* allFrames;
		FrameData* currentFrame;

		void RenderFrame()	override;

		void BuildScenePipelines(VulkanMesh* m);
		void BuildDebugPipelines();

		void UpdateObjectList();
		void UpdateDebugData();
		 
		void RenderSceneObjects(VulkanPipeline& pipe, vk::CommandBuffer cmds);
		void RenderSkybox(vk::CommandBuffer cmds);
		void RenderDebugLines(vk::CommandBuffer cmds);
		void RenderDebugText(vk::CommandBuffer cmds);

		UniqueVulkanMesh GenerateQuad();

		GameWorld& gameWorld;
		vector<const RenderObject*> activeObjects;
		int currentFrameIndex;

		VulkanPipeline	skyboxPipeline;
		VulkanPipeline	shadowPipeline;
		VulkanPipeline	scenePipeline;
		VulkanPipeline	debugLinePipeline;
		VulkanPipeline	debugTextPipeline;

		UniqueVulkanShader skyboxShader;
		UniqueVulkanShader shadowShader;
		UniqueVulkanShader sceneShader;

		UniqueVulkanShader debugLineShader;
		UniqueVulkanShader debugTextShader;

		UniqueVulkanMesh	quadMesh;
		UniqueVulkanTexture	cubeTex;

		UniqueVulkanTexture shadowMap;
		UniqueVulkanTexture defaultTexture;
	
		vk::Viewport	shadowViewport;
		vk::Rect2D		shadowScissor;

		vk::UniqueDescriptorSetLayout globalDataLayout;
		vk::UniqueDescriptorSetLayout objectTextxureLayout;
		vk::UniqueDescriptorSet objectTextxureDescriptor;

		vk::UniqueSampler	defaultSampler;
		vk::UniqueSampler	textSampler;

		vector<VulkanTexture*> loadedTextures;
	};
}
#endif

