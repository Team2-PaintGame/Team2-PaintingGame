#pragma once
#ifdef _WIN32
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
			LoadingScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL);
			void OnAwake() override;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void SetThread(std::thread* t) { threadToWait = t; }
			RenderObject* GetLoader() { return (RenderObject *) hUDOnLoad; }

		protected:
			virtual PushdownResult onStateChange(PushdownState** newState);
			void MenuFrame() override {}
			std::thread* threadToWait = nullptr;

			RenderObject* renderObj;
			Quaternion quat;
			float timer = 0;
			Transform* transform;
			HUDOnLoad* hUDOnLoad;
			bool bLoaded = false;
			
		};

	}
}
#endif
