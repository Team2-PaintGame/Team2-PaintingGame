#pragma once
#ifdef _WIN32
#include <OGLRenderer.h>
#include <glad/gl.h>
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		//class RenderObject;
		struct DebugLinesRenderer {
			GLuint vao;
			GLuint vertVbo;
			size_t count;

			DebugLinesRenderer() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vertVbo);
			}
			~DebugLinesRenderer() {
				glDeleteVertexArrays(1, &vao);			//Delete our VAO
				glDeleteBuffers(1, &vertVbo);			//Delete our VBO
			}
		};
		struct DebugTextRenderer {
			GLuint vao;
			GLuint vertVBO;
			GLuint colourVBO;
			GLuint texCoordVBO;
			size_t count;

			DebugTextRenderer() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vertVBO);
				glGenBuffers(1, &colourVBO);
				glGenBuffers(1, &texCoordVBO);

			}
			~DebugTextRenderer() {
				glDeleteVertexArrays(1, &vao);			//Delete our VAO
				glDeleteBuffers(1, &vertVBO);			//Delete our VBO
				glDeleteBuffers(1, &colourVBO);			//Delete our VBO
				glDeleteBuffers(1, &texCoordVBO);		//Delete our VBO
			}
		};
		/*class RendererSettings {
		public:
			class DebugRendererSettings {
			public:
				DebugRendererSettings() {}
				void SetIsCollisionShapeDisplayed(bool boolean) {
					debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, boolean);
				}
				void SetIsBroadPhaseAABBDisplayed(bool boolean) {
					debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, boolean);
				}
				reactphysics3d::DebugRenderer* debugRenderer;
			private:
				bool isCollisionShapeEnabled = false;
				bool isBroadPhaseAABBEnabled = false;
			};
			RendererSettings() {};

			void SetIsWireFrameModeEnabled(bool boolean) {
				isWireFrameModeEnabled = boolean;
			}
			void SetIsDebugRenderingModeEnabled(bool boolean) {
				physicsWorld->setIsDebugRenderingEnabled(boolean);
			}
			bool GetIsWireFrameModeEnabled() {
				return isWireFrameModeEnabled;
			}
			bool GetIsDebugRenderingModeEnabled() {
				return physicsWorld->getIsDebugRenderingEnabled();
			}
			DebugRendererSettings debugRendererSettings;
		private:
			bool isWireFrameModeEnabled = false;
			bool isSplitScreen = false;
			reactphysics3d::DebugRenderer* debugRenderer;
		};*/
		class OGLPaintingGameRenderer : public OGLRenderer {
		public:
			OGLPaintingGameRenderer(Window& w);
			~OGLPaintingGameRenderer();
			virtual void BindScreen(void* screen) { boundScreen = (BaseScreen*)screen; };
			//RendererSettings settings, skybox, shadow!!!!!!!!!!!! lines, debug, set player class and its camera, fix camera class.
		protected:
			void RenderFrame()	override;
			//void RenderCamera();
			//remove these functions from here, gui library should have its own instance
			void CreateImGuiContext();
			void DeleteImGuiContext();

			void RenderBasicScreen();
			void RenderGameScreen();

			void BuildObjectList();
			void SortObjectList();
			vector<const RenderObject*> activeObjects;
			BaseScreen* boundScreen;
		};
		
		// Concrete factory for creating Painting Game OpenGL renderer
		class OGLPaintingGameRendererFactory : public OGLRendererFactory {
		public:
			RendererBase* createRenderer(Window& w) override {
				return new OGLPaintingGameRenderer(w);
			}
		};
	}
}
#endif
