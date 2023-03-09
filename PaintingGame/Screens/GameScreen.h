#pragma once
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		class GameScreen : public BaseScreen
		{
		public:
			GameScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL) : BaseScreen(screenManager, sceneNode) {
				screenType = ScreenType::GameScreen;
				toggleRenderDebug = false;
			}
			void OnAwake() override;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			static void GamePauseCallback();

			bool GetRenderDebugLines() override { return toggleRenderDebug; } 
		protected:
			void LoadGame();
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);

		protected:
			bool toggleRenderDebug;
			static bool sPauseCallback;
		};

	}
}
