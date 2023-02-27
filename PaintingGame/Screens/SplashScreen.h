#pragma once
#include "BaseScreen.h"

namespace NCL {
	namespace CSC8508 {
		class SplashScreen : public BaseScreen
		{
		public:
			SplashScreen(SceneNode* sceneNode) : BaseScreen(sceneNode) {}
			virtual ~SplashScreen();
			void OnAwake() override;
		protected:
			virtual void MenuFrame();
			virtual PushdownResult onStateChange(PushdownState** newState);
		};

	}
}
