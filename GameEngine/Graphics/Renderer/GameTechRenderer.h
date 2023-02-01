#pragma once
#include "OGLRenderer.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "OGLMesh.h"
#include "OGLSkybox.h"

#include "GameWorld.h"
#include "TextureHUD.h"


namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;
	namespace CSC8508 {
		class RenderObject;

		class GameTechRenderer : public OGLRenderer	{
		public:

			class RendererSettings {
			public:
				class DebugRendererSettings {
				public:
					DebugRendererSettings(reactphysics3d::PhysicsWorld* physicsWorld) : debugRenderer(physicsWorld->getDebugRenderer()) {}
					void SetIsCollisionShapeDisplayed(bool boolean) {
						debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, boolean);
					}
					void SetIsBroadPhaseAABBDisplayed(bool boolean) {
						debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, boolean);
					}
					reactphysics3d::DebugRenderer& debugRenderer;
				private:
					bool isCollisionShapeEnabled = false;
					bool isBroadPhaseAABBEnabled = false;
				};
				RendererSettings(reactphysics3d::PhysicsWorld* physicsWorld) : physicsWorld(physicsWorld), debugRendererSettings(physicsWorld) {};

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
				reactphysics3d::PhysicsWorld* physicsWorld;
			};
			GameTechRenderer(GameWorld& world, reactphysics3d::PhysicsWorld* physicsWorld);
			~GameTechRenderer();
			virtual void Update(float dt);
			
			MeshGeometry*	LoadMesh(const string& name);
			MeshGeometry* LoadFlatMesh(int hVertexCount = 128, int wVertexCount = 128);
			MeshGeometry* LoadHeightMap(const std::string& filename, int heightMultiplier = 10);
			TextureBase*	LoadTexture(const string& name);
			ShaderBase*		LoadShader(const string& vertex, const string& fragment);
			void AddHudTextures(const string& name, const Vector2& position, const Vector2& scale);

			void UseFog(bool val) { useFog = val; }

			RendererSettings settings;
		protected:
			void NewRenderLines();
			void NewRenderText();

			void RenderFrame()	override;

			OGLShader*		defaultShader;

			GameWorld&	gameWorld;

			void BuildObjectList();
			void SortObjectList();
			void RenderShadowMap();
			void RenderCamera(); 
			void RenderSkybox();
			void RenderSky();
			void RenderHUD();
			void RenderDebugInformation(); 

			void SetDebugStringBufferSizes(size_t newVertCount);
			void SetDebugLineBufferSizes(size_t newVertCount);

			vector<const RenderObject*> activeObjects;

			OGLShader*  debugShader;
			OGLSkybox* skybox;

			//shadow mapping things
			OGLShader*	shadowShader;
			GLuint		shadowTex;
			GLuint		shadowFBO;
			Matrix4     shadowMatrix;

			Vector4		lightColour;
			float		lightRadius;
			Vector3		lightPosition;

			//Debug data storage things
			vector<Vector3> debugLineData;

			vector<Vector3> debugTextPos;
			vector<Vector4> debugTextColours;
			vector<Vector2> debugTextUVs;

			GLuint lineVAO;
			GLuint lineVertVBO;
			size_t lineCount;

			GLuint textVAO;
			GLuint textVertVBO;
			GLuint textColourVBO;
			GLuint textTexVBO;
			size_t textCount;

			//hud things
			OGLMesh* quadModel;
			OGLShader* hudShader;
			vector<TextureHUD> hudTextures;
			bool useFog = false;
			Vector3 fogColour = Vector3(0.6706f, 0.6824f, 0.6902f); //removing alpha value of fog colour to preserve the original transparency value of the fragment
		};
	}
}

