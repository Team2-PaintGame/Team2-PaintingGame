#pragma once
#include "ScreenManager.h"
#include <thread>

namespace NCL {
	namespace CSC8508 {
		class LoadingScreen : public BaseScreen
		{
		public:
			LoadingScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL) : BaseScreen(screenManager, sceneNode) {
				screenType = ScreenType::LoadingScreen;
			}
			void OnAwake() override;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

		protected:
			virtual PushdownResult onStateChange(PushdownState** newState);
			void MenuFrame() override {}
			std::thread* gameScreenThread = nullptr;
			
		};

	}
}
