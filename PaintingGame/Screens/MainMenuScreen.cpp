#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

void MainMenuScreen::OnAwake() {
	currentScreen = ScreenType::MainMenuScreen;
}

void MainMenuScreen::MenuFrame() {
	ImGui::Begin("Main Menu");
	if (ImGui::Button("Single Player Game")) {
		nextScreen = ScreenType::GameScreen;
		gameModeSelected = GameMode::SinglePlayer;
	}
	if (ImGui::Button("Split Screen: 2 Player Game")) {
		nextScreen = ScreenType::GameScreen;
		gameModeSelected = GameMode::SplitScreen;
	}
	if (ImGui::Button("LAN Game")) {
		nextScreen = ScreenType::GameScreen;
		gameModeSelected = GameMode::Networked;
	}	
	if (ImGui::Button("Exit")) {
		//nextScreen = ScreenType::Exit;
	}
	ImGui::End();
}

PushdownState::PushdownResult MainMenuScreen::onStateChange(PushdownState** newState) {
	switch (nextScreen) {
		case ScreenType::GameScreen: {
			//*newState = new GameScreen(window, renderer, gameWorld, physicsCommon, menuHandler);
			//how to tell game screen to construct single, multi, networked game in its constructor
			return PushdownResult::Push;
		}

		default: {
			return PushdownResult::NoChange;
		}
	}
}
