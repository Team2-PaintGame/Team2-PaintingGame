#pragma once
#ifdef __ORBIS__
#include <GNMRenderer.h>
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {

		class GNMPaintingGameRenderer : public GNMRenderer {
		public:
			GNMPaintingGameRenderer(Window& w) {}
			virtual ~GNMPaintingGameRenderer() {}
		protected:

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
