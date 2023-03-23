#pragma once
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		class MainMenuScreen : public BaseScreen
		{
		public:
			MainMenuScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {
				screenType = ScreenType::MainMenuScreen;
			}
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
			void TransitionTimer(float dt) override;
		};

	}
}
