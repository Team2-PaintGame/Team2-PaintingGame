#pragma once
#include <RendererBase.h>
#include <AssetLoader.h>
#include "OGLPaintingGameRenderer.h"
#include <GNMRenderer.h>
#include <OGLAssetLoader.h>
#include <GNMAssetLoader.h>

namespace NCL {
	namespace CSC8508 {
		
		struct PlatformConfigurations {
			PlatformConfigurations() {
				// Determine which platform the user is on
#ifdef _WIN32
				rendererFactory = new OGLPaintingGameRendererFactory();
				assetLoaderFactory = new Assets::OGLAssetLoaderFactory();
#endif
#ifdef __ORBIS__
				rendererFactory = new GNMPaintingGameRendererRendererFactory();
				assetLoaderFactory = new Assets::GNMAssetLoaderFactory();
#endif
			}
			~PlatformConfigurations() {
				delete rendererFactory;
				delete assetLoaderFactory;
			}
			RendererFactory* rendererFactory;
			Assets::AssetLoaderFactory* assetLoaderFactory;
		};
	}
}

