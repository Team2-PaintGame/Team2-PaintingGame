#pragma once
#ifdef __ORBIS__
#include <GNMRenderer.h>
#include "ScreenManager.h"

namespace NCL {
	using namespace GNM;
	namespace CSC8508 {
		struct DebugLinesRenderer {
			DebugLinesRenderer() {}
			~DebugLinesRenderer() {}
		};
		struct DebugTextRenderer {
			DebugTextRenderer() {}
			~DebugTextRenderer() {}
		};

		struct ShaderBuffers {
			Gnm::Buffer	cameraBuffer;
			Gnm::Buffer objBuffer;
		};
		
		struct RenderObjectVariables {
			Matrix4 modelMatrix;
			bool hasVertexColours;
			Vector4 objectColour;
		};

		struct CameraVariables {
			Matrix4 viewProjMatrix;
			Vector3 cameraPos;
		};

		struct ShaderVariables {
			CameraVariables* cameraVariables;
			RenderObjectVariables* renderObjectVariables;
		};

		class GNMPaintingGameRenderer : public GNMRenderer {
		public:
			GNMPaintingGameRenderer(Window& w);
			virtual ~GNMPaintingGameRenderer();
			virtual void BindScreen(void* screen) override { boundScreen = (BaseScreen*)screen; };
			void BindDebugShader(ShaderBase* dShader) override { debugShader = dShader; }
		protected:
			void RenderFrame()	override;

			void RenderBasicScreen();
			void RenderGameScreen();

			void RenderPaintSplat(GNMShader* shader);

			void BuildObjectList();
			void SortObjectList();

			void SendModelMatrices(GNMShader* shader, const RenderObject* r);
			void RenderWithDefaultTexture(const RenderObject* r);
			void RenderWithMultipleTexture(const RenderObject* r);

			//Debug methods
			void RenderDebugInformation() const;
			void NewRenderLines();
			void NewRenderText();
			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			vector<const RenderObject*> activeObjects;
			BaseScreen* boundScreen;
			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;
			ShaderBase* debugShader = nullptr;

			//Debug data storage things
			vector<Vector3> debugLineData;
			vector<Vector3> debugTextPos;
			vector<Vector4> debugTextColours;
			vector<Vector2> debugTextUVs;

			DebugTextRenderer debugTextRenderer;
			DebugLinesRenderer debugLineRenderer;

			ShaderBuffers shaderBuffers;
			ShaderVariables shaderVariables;
		};

		// Concrete factory for creating Painting Game OpenGL renderer
		class GNMPaintingGameRendererFactory : public GNMRendererFactory {
		public:
			RendererBase* createRenderer(Window& w) override {
				return new GNMPaintingGameRenderer(w);
			}
		};
	}
}
#endif
