#include "SplashScreen.h"

using namespace NCL;
using namespace CSC8508;

void SplashScreen::OnAwake() {
	currentScreen = ScreenType::SplashScreen;
}

void SplashScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Play Game")) {
		//transition to main menu
		nextScreen = ScreenType::MainMenuScreen;
	}
	ImGui::End();
}

PushdownState::PushdownResult SplashScreen::onStateChange(PushdownState** newState) {
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
