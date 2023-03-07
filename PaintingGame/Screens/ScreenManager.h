#pragma once
#include "../PaintingGame/GameAssets.h"
#include "SceneNode.h"
#include "PushdownState.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>
#include "PushdownMachine.h"

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
		CreateNetworkedGameAsServer,
		CreateNetworkedGameAsClient,
		Exit,
	};
	class BaseScreen;
	class ScreenManager {
	public:
		ScreenManager(GameAssets* assets);
		BaseScreen* GetScreen(ScreenType screenType) const;
		BaseScreen* GetActiveScreen() const;
		GameAssets* GetGameAssets() const { return assets; }
		bool Update(float dt);
	protected:
		void LoadScreens();
		std::unordered_map<ScreenType, std::unique_ptr<BaseScreen>> screens;
		std::unordered_map<ScreenType, std::unique_ptr<SceneNode>> screenSceneNodes;
		GameAssets* assets;
		std::unique_ptr<PushdownMachine> machine;
	};
	class BaseScreen : public PushdownState
	{
	public:
		BaseScreen(ScreenManager* screenManager, SceneNode* sceneNode = NULL) : screenManager(screenManager), sceneNode(sceneNode) {}
		virtual ~BaseScreen() {}
		PushdownResult OnUpdate(float dt, PushdownState** newState) override;
		void OnAwake() override {};
		void SetCommand(ScreenCommand command) { this->command = command; };
		SceneNode* GetSceneNode() const { return sceneNode; }
		ScreenType GetScreenType() const { return screenType; }
		void RenderMenu();
	protected:
		virtual void MenuFrame() = 0;
		virtual PushdownResult onStateChange(PushdownState** newState) = 0;
		SceneNode* sceneNode = NULL;
		bool isMenuDisplayed = true;
		ScreenCommand command = ScreenCommand::None;
		ScreenManager* screenManager;
		ScreenType screenType = ScreenType::None;
	};
}