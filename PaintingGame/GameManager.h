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
		void calculateRenderingTime();
#ifdef _WIN32
		static void LoadAssets(GameAssets* assets, OGLRenderer* renderer, HGLRC context);
		static void FinishLoadingCallback();
		static bool GetLoadingFlag() { return loadingFlag; }
#endif
		static PlatformConfigurations sConfig;
	protected:
		void FinishLoading();

		RendererBase* renderer;
		Assets::AssetLoader* assetLoader;
		GameAssets* gameAssets;
		ScreenManager* screenManager;

		static GameManager* gameManager;
		static bool loadingFlag;

		std::thread assetThread;
	};
}