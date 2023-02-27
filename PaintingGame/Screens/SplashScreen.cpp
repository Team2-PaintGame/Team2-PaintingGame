#include "SplashScreen.h"
#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

void SplashScreen::OnAwake() {
	currentScreen = Screen::SplashScreen;
}

void SplashScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Play Game")) {
		//transition to main menu
		nextScreen = Screen::MainMenuScreen;
	}
	ImGui::End();
}

PushdownState::PushdownResult SplashScreen::onStateChange(PushdownState** newState) {
	switch (nextScreen) {
		case Screen::MainMenuScreen: {
			*newState = new MainMenuScreen();
			return PushdownResult::Push;
		}

		default: {
			return PushdownResult::NoChange;
		}
	}
}
