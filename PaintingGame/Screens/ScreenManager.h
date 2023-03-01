#pragma once
#include "GameAssets.h"
#include "SceneNode.h"
#include "PushdownState.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

namespace NCL::CSC8508 {
	enum class ScreenType {
		None,
		SplashScreen,
		MainMenuScreen,
		GameScreen,
		GameWinScreen,
		GameOverScreen,
	};
	class BaseScreen;
	class ScreenManager {
	public:
		ScreenManager(GameAssets* assets);
		BaseScreen* GetScreen(ScreenType screenType) const;
	protected:
		std::unordered_map<ScreenType, std::unique_ptr<BaseScreen>> screens;
		std::unordered_map<ScreenType, SceneNode> screenSceneNodes;
	};
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