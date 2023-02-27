#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

void MainMenuScreen::OnAwake() {
	currentScreen = Screen::MainMenuScreen;
}

void MainMenuScreen::MenuFrame() {
	ImGui::Begin("Main Menu");
	bool isSinglePlayer = ImGui::Button("Single Player Game");
	bool isSplitScreen = ImGui::Button("Split Screen: 2 Player Game");
	bool isNetworkedGame = ImGui::Button("LAN Game");
	bool isExit = ImGui::Button("Exit");
	if (isSinglePlayer || isSplitScreen || isNetworkedGame) {
		nextScreen = Screen::GameScreen;
	}
	else if (isExit) {
		nextScreen
	}
	ImGui::End();
}

PushdownState::PushdownResult MainMenuScreen::onStateChange(PushdownState** newState) {
	switch (nextScreen) {
		case Screen::GameScreen: {
			//*newState = new GameScreen(window, renderer, gameWorld, physicsCommon, menuHandler);
			//how to tell game screen to construct single, multi, networked game in its constructor
			return PushdownResult::Push;
		}

		default: {
			return PushdownResult::NoChange;
		}
	}
}
