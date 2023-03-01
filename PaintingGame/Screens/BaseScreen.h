#pragma once
#include "PushdownState.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include "SceneNode.h"
#include "ScreenManager.h"

namespace NCL {
	namespace CSC8508 {
		class BaseScreen : public PushdownState
		{
		public:
			BaseScreen(ScreenManager* screenManager, SceneNode* sceneNode) : screenManager(screenManager), sceneNode(sceneNode) {}
			virtual ~BaseScreen() {}
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override = 0;
		protected:
			//void RenderMenu();
			virtual void MenuFrame() = 0;
			virtual PushdownResult onStateChange(PushdownState** newState) = 0;
			SceneNode* sceneNode = NULL;
			bool isMenuDisplayed = true;
			ScreenType currentScreen = ScreenType::None;
			ScreenType nextScreen = ScreenType::None;
			ScreenManager* screenManager;
		};

	}
}
