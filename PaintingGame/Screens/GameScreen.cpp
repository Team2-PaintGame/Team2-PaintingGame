#include "GameScreen.h"

using namespace NCL;
using namespace CSC8508;

void GameScreen::OnAwake() {
	currentScreen = ScreenType::SplashScreen;
}

void GameScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Play Game")) {
		//transition to main menu
		nextScreen = ScreenType::MainMenuScreen;
	}
	ImGui::End();
}

PushdownState::PushdownResult GameScreen::onStateChange(PushdownState** newState) {
	switch (nextScreen) {
	case ScreenType::MainMenuScreen: {
		*newState = screenManager->GetScreen(ScreenType::MainMenuScreen);
		return PushdownResult::Push;
	}

	default: {
		return PushdownResult::NoChange;
	}
	}
}
