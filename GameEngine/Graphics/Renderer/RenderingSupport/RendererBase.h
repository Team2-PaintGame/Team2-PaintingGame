/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "Window.h"
#include "ShaderBase.h"
#include "TextureBase.h"
#include "MeshGeometry.h"

namespace NCL::Rendering {
	enum class VerticalSyncState {
		VSync_ON,
		VSync_OFF,
		VSync_ADAPTIVE
	};

	// The abstract factory interface for creating renderer objects
	class RendererFactory {
	public:
		virtual ~RendererFactory() {}
		virtual RendererBase* createRenderer(Window& w) = 0;
	};

	class RendererBase {
	public:
		friend class NCL::Window;

		RendererBase(Window& w);
		virtual ~RendererBase();

		bool HasInitialised() const  {
			return initState;
		}

		void ForceValidDebugState(bool newState) {
			forceValidDebugState = newState;
		}

		virtual void Update(float dt) {}

		void Render() {
			BeginFrame();
			RenderFrame();
			EndFrame();
			SwapBuffers();
		}

		virtual bool SetVerticalSync(VerticalSyncState s) {
			return false;
		}

	protected:
		virtual void OnWindowResize(int w, int h) = 0;
		virtual void OnWindowDetach() {}; //Most renderers won't care about this
			
		virtual void BeginFrame()	= 0;
		virtual void RenderFrame()	= 0;
		virtual void EndFrame()		= 0;
		virtual void SwapBuffers()	= 0;

		virtual void BindShader(ShaderBase* s) = 0;
		virtual void BindTextureToShader(const TextureBase* t, const std::string& uniform, int texUnit) const = 0;
		virtual void BindMesh(MeshGeometry* m) = 0;
		virtual void DrawBoundMesh(int subLayer = 0, int numInstances = 1) = 0;

		Window& hostWindow;

		int windowWidth;
		int windowHeight;

		bool initState;
		bool forceValidDebugState;
	};
}