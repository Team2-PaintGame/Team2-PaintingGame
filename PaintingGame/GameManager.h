#pragma once
#include "ScreenManager.h"
#include "PlatformConfigurations.h"
#include "GameAssets.h"
#include <PushdownMachine.h>

namespace NCL::CSC8508 {
	class GameManager {
	public:
		GameManager(Window* window) {
			renderer = config.rendererFactory->createRenderer(*window);
			assetLoader = config.assetLoaderFactory->createAssetLoader();
			gameAssets = new GameAssets(assetLoader);
			screenManager = new ScreenManager(gameAssets);
			gameMachine = new PushdownMachine((PushdownState*)screenManager->GetScreen(ScreenType::SplashScreen));
		}
		~GameManager() {
			delete renderer;
			delete screenManager;
			delete assetLoader;
			delete gameAssets;
			delete gameMachine;
		}
		void Run(Window* window);
		//setup renderer, load imgui context inside renderer
	protected:
		PlatformConfigurations config;
		RendererBase* renderer;
		Assets::AssetLoader* assetLoader;
		GameAssets* gameAssets;
		ScreenManager* screenManager;
		PushdownMachine* gameMachine;
	};
}