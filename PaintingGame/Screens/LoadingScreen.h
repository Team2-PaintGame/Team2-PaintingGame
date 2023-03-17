#pragma once
#include "ScreenManager.h"
#include "Quaternion.h"
#include "Transform.h"
#include <SceneNode.h>
#include <thread>

namespace NCL {
	namespace CSC8508 {
		class HUDOnLoad;
		class LoadingScreen : public BaseScreen
		{
		public:
			LoadingScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL) : BaseScreen(screenManager, sceneNode) {
				screenType = ScreenType::LoadingScreen;
				renderObj = sceneNode->GetRenderObject();
				transform = renderObj->GetTransform();
				int a = 0;
			}
			void OnAwake() override;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			HUDOnLoad* hUDOnLoad;

		protected:
			virtual PushdownResult onStateChange(PushdownState** newState);
			void MenuFrame() override {}
			std::thread* gameScreenThread = nullptr;

			RenderObject* renderObj;
			Quaternion quat;
			float timer = 0;
			Transform* transform;
			
			
		};

	}
}
