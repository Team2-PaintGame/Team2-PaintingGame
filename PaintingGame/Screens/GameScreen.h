#pragma once
#include "BaseScreen.h"

namespace NCL {
	namespace CSC8508 {
		class GameScreen : public BaseScreen
		{
		public:
			GameScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {}
			//virtual ~SplashScreen();
			void OnAwake() override;
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
		};

	}
}
