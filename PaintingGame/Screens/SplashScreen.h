#pragma once
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		class SplashScreen : public BaseScreen
		{
		public:
			SplashScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {}
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
		};

	}
}
