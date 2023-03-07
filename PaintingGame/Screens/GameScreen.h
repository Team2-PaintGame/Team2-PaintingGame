#pragma once
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		class GameScreen : public BaseScreen
		{
		public:
			GameScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL) : BaseScreen(screenManager, sceneNode) {
				screenType = ScreenType::GameScreen;
			}
			void OnAwake() override;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			static void GamePauseCallback();
		protected:
			void LoadGame();
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);

		protected:
			static bool s_pauseCallback;
		};

	}
}
