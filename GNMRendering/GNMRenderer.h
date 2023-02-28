/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "RendererBase.h"

#include "Vector3.h"
#include "Vector4.h"


#ifdef _WIN32
#include "windows.h"
#endif

#ifdef _DEBUG
#define OPENGL_DEBUGGING
#endif


#include <string>
#include <vector>

namespace NCL {
	class MeshGeometry;

	namespace Maths {
		class Matrix4;
	}

	namespace Rendering {
		class ShaderBase;
		class TextureBase;

		class GNMMesh;
		class GNMShader;

		class GNMRenderer : public RendererBase
		{
		public:
			friend class OGLRenderer;
			GNMRenderer(Window& w);
			~GNMRenderer();

			void OnWindowResize(int w, int h)	override;

			virtual bool SetVerticalSync(VerticalSyncState s);

			/*void DrawString(const std::string& text, const Vector2&pos, const Vector4& colour = Vector4(0.75f, 0.75f, 0.75f,1), float size = 20.0f );
			void DrawLine(const Vector3& start, const Vector3& end, const Vector4& colour);*/

		protected:			
			void BeginFrame()	override;
			void RenderFrame()	override;
			void EndFrame()		override;
			void SwapBuffers()  override;

			/*void DrawDebugData();
			void DrawDebugStrings();
			void DrawDebugLines();*/

			virtual void BindShader(ShaderBase*s);
			virtual void BindTextureToShader(const TextureBase*t, const std::string& uniform, int texUnit) const;
			virtual void BindMesh(MeshGeometry*m);
			virtual void DrawBoundMesh(int subLayer = 0, int numInstances = 1);
#ifdef _WIN32
			void InitWithWin32(Window& w);
			void DestroyWithWin32();
			HDC		deviceContext;		//...Device context?
			HGLRC	renderContext;		//Permanent Rendering Context		
#endif
		private:
		
			GNMMesh*	boundMesh;
			GNMShader*	boundShader;
		};

		// Concrete factory for creating GNM renderer
		class GNMRendererFactory : public RendererFactory {
		public:
			RendererBase* createRenderer(Window& w) override {
				return new GNMRenderer(w);
			}
		};

	}
}