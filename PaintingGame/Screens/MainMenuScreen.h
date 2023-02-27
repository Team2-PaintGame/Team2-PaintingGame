#pragma once
#include "BaseScreen.h"

namespace NCL {
	namespace CSC8508 {
		class MainMenuScreen : public BaseScreen
		{
		public:
			MainMenuScreen(SceneNode* sceneNode) : BaseScreen(sceneNode) {}
			virtual ~MainMenuScreen();
			void OnAwake() override;
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
		};

	}
}
