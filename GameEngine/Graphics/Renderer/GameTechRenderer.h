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
	namespace CSC8503 {
		class RenderObject;

		class GameTechRenderer : public OGLRenderer	{
		public:
			GameTechRenderer(GameWorld& world);
			~GameTechRenderer();
			virtual void Update(float dt);
			
			MeshGeometry*	LoadMesh(const string& name);
			MeshGeometry* LoadFlatMesh(int hVertexCount = 128, int wVertexCount = 128);
			MeshGeometry* LoadHeightMap(const std::string& filename, int heightMultiplier = 10);
			TextureBase*	LoadTexture(const string& name);
			ShaderBase*		LoadShader(const string& vertex, const string& fragment);
			void AddHudTextures(const string& name, const Vector2& position, const Vector2& scale);

			void UseFog(bool val) { useFog = val; }
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

