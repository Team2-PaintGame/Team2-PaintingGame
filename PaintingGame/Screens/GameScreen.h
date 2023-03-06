#pragma once
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		class GameScreen : public BaseScreen
		{
		public:
			GameScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {}
			void OnAwake() override;
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
		};

	}
}
