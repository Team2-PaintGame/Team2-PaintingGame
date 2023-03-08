#include "SplashScreen.h"

using namespace NCL;
using namespace CSC8508;

void SplashScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Play Game")) {
		command = ScreenCommand::TransitionToNextScreen;
	}
	if (ImGui::Button("Exit")) {
		command = ScreenCommand::Exit;
	}
	ImGui::End();
}

PushdownState::PushdownResult SplashScreen::onStateChange(PushdownState** newState) {
	switch (command)
	{
		case ScreenCommand::TransitionToNextScreen: 
			command = ScreenCommand::None;
			*newState = screenManager->GetScreen(ScreenType::MainMenuScreen);
			return PushdownResult::Push;
		case ScreenCommand::Exit: 
			command = ScreenCommand::None;
			return PushdownResult::Pop;
		default: 
			return PushdownResult::NoChange;
	}
}
