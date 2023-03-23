#pragma once
#include "ScreenManager.h"
#include <string>

namespace NCL {
	namespace CSC8508 {
		class GameOverScreen : public BaseScreen
		{
		public:
			GameOverScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {
				screenType = ScreenType::GameOverScreen;
			}

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;


			void SetWinner(int win) {
				winner = win;
			}
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
			int winner;
			std::string victory_string;
		};

	}
}
