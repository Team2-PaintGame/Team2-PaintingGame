#pragma once
#include "ScreenManager.h"
#include "PlatformConfigurations.h"
#include "GameAssets.h"
#include <PushdownMachine.h>

namespace NCL::CSC8508 {
	class GameManager {
	public:
		GameManager(Window* window);
		~GameManager();
		bool RunGame(float dt);
	protected:
		PlatformConfigurations config;
		RendererBase* renderer;
		Assets::AssetLoader* assetLoader;
		GameAssets* gameAssets;
		ScreenManager* screenManager;
		
	};
}