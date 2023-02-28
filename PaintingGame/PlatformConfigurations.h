#pragma once
#include "PaintingGame.h"

namespace NCL {
	namespace CSC8508 {
		
		struct PlatformConfigurations {
			PlatformConfigurations() {
				// Determine which platform the user is on
#ifdef _WIN32
				rendererFactory = new OGLRendererFactory();
#endif
#ifdef __ORBIS__
				rendererFactory = new GNMRendererFactory();
#endif
			}
			~PlatformConfigurations() {
				delete rendererFactory;
			}
			RendererFactory* rendererFactory;
		};
	}
}

