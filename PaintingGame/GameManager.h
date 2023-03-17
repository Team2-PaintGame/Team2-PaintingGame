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
		static void LoadAssets(GameAssets* assets);
		static PlatformConfigurations sConfig;
	protected:
		RendererBase* renderer;
		Assets::AssetLoader* assetLoader;
		GameAssets* gameAssets;
		ScreenManager* screenManager;
		std::thread assetThread;
	};
}