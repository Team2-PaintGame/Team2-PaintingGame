#pragma once
#ifdef __ORBIS__
#include "RendererBase.h"
#include "PS4MemoryAware.h"
#include <gnm.h>
#include <gnmx\fetchshaderhelper.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\allocators.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\stack_allocator.h>
#include <..\samples\sample_code\graphics\api_gnm\toolkit\toolkit.h>
#include "PS4Frame.h"
#include "GNMTexture.h"

using namespace sce;
using namespace Gnmx;
using namespace Gnmx::Toolkit;

namespace NCL::Maths {
	class Matrix4;
}

namespace NCL::GNM {
	using namespace PS4;
	enum MemoryLocation {
		MEMORY_GARLIC,
		MEMORY_ONION,
		MEMORY_MAX
	};

	struct GNMScreenBuffer {
		sce::Gnm::RenderTarget		colourTarget;
		sce::Gnm::DepthRenderTarget depthTarget;
	};

	class PS4Window;
	class GNMShader;
	class GNMMesh;

	class RenderObject;

	class GNMRenderer :
		public RendererBase, public PS4MemoryAware
	{
	public:
		GNMRenderer(Window& window);
		~GNMRenderer();

	protected:
		void	OnWindowResize(int w, int h) override;
		void	BeginFrame()    override;
		void	RenderFrame()    override;
		void	EndFrame()		override;
		void	SwapBuffers()	override;

		void	SwapScreenBuffer();
		void	SwapCommandBuffer();
		void	SetRenderBuffer(GNMScreenBuffer* buffer, bool clearColour, bool clearDepth, bool clearStencil);
		void	ClearBuffer(bool colour, bool depth, bool stencil);
		
		GNMScreenBuffer* GenerateScreenBuffer(uint width, uint height, bool colour = true, bool depth = true, bool stencil = false);

	private:
		void	InitialiseMemoryAllocators();
		void	InitialiseVideoSystem();
		void	InitialiseGCMRendering();

		void	DestroyMemoryAllocators();
		void	DestroyVideoSystem();
		void	DestroyGCMRendering();

	protected:
		int currentGPUBuffer;

		const int _MaxCMDBufferCount;

		//VIDEO SYSTEM VARIABLES
		int videoHandle;		//Handle to video system

		//SCREEN BUFFER VARIABLES
		const int			_bufferCount;	//How many screen buffers should we have
		int					currentScreenBuffer;
		int					prevScreenBuffer;
		GNMScreenBuffer** screenBuffers;	//Pointer to our screen buffers
		//Memory Allocation
		const int _GarlicMemory;
		const int _OnionMemory;

		sce::Gnmx::Toolkit::StackAllocator	stackAllocators[MEMORY_MAX];

		//Individual Frames
		PS4Frame* frames;

		int framesSubmitted;

		//Per frame pointers...
		GNMScreenBuffer* currentGNMBuffer;  //Pointer to whichever buffer we're currently using...
		Gnmx::GnmxGfxContext* currentGFXContext;
		PS4Frame* currentFrame;
	};

	// Concrete factory for creating GNM renderer
	class GNMRendererFactory : public RendererFactory {
	public:
		RendererBase* createRenderer(Window& w) override {
			return new GNMRenderer(w);
		}
	};
}
#endif
