#pragma once
#include <RendererBase.h>
#include <AssetLoader.h>
#include "OGLPaintingGameRenderer.h"
#include <GNMPaintingGameRenderer.h>
#include <OGLAssetLoader.h>
#include <GNMAssetLoader.h>
#include "PlayerController.h"
#include "PlayerControllers.h"

namespace NCL {
	namespace CSC8508 {
		
		struct PlatformConfigurations {
			PlatformConfigurations() {
				// Determine which platform the user is on
#ifdef _WIN32
				rendererFactory = new OGLPaintingGameRendererFactory();
				assetLoaderFactory = new Assets::OGLAssetLoaderFactory();
				//not assigning player controller factory for windows platform, 
				//it can either be win32 or xbox depending on type of game selected
#endif
#ifdef __ORBIS__
				rendererFactory = new GNMPaintingGameRendererFactory();
				assetLoaderFactory = new Assets::GNMAssetLoaderFactory();
				playerControllerFactory = new PS4ControllerFactory();
#endif
			}
			~PlatformConfigurations() {
				delete rendererFactory;
				delete assetLoaderFactory;
				delete playerControllerFactory;
			}
			RendererFactory* rendererFactory = NULL;
			Assets::AssetLoaderFactory* assetLoaderFactory = NULL;
			PlayerControllerFactory* playerControllerFactory = NULL;
		};
	}
}

