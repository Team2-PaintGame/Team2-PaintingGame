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
	enum class ScreenCommand {
		None,
		TransitionToNextScreen,
		TransitionToPreviousScreen,
		CreateSinglePlayerGame,
		CreateSplitScreenGame,
		CreateNetworkedGame,
		Exit,
	};
	class BaseScreen;
	class ScreenManager {
	public:
		ScreenManager(GameAssets* assets);
		BaseScreen* GetScreen(ScreenType screenType) const;
		GameAssets* GetGameAssets() const { return assets; }
	protected:
		std::unordered_map<ScreenType, std::unique_ptr<BaseScreen>> screens;
		std::unordered_map<ScreenType, SceneNode> screenSceneNodes;
		GameAssets* assets;
	};
	class BaseScreen : public PushdownState
	{
	public:
		BaseScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL) : screenManager(screenManager), sceneNode(sceneNode) {}
		virtual ~BaseScreen() {}
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override {};
		void SetCommand(ScreenCommand command) { this->command = command; };
	protected:
		//void RenderMenu();
		virtual void MenuFrame() = 0;
		virtual PushdownResult onStateChange(PushdownState** newState) = 0;
		SceneNode* sceneNode = NULL;
		bool isMenuDisplayed = true;
		ScreenCommand command = ScreenCommand::None;
		ScreenManager* screenManager;
	};
}