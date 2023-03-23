#include "SplashScreen.h"
#include "Debug.h"

using namespace NCL;
using namespace CSC8508;

void SplashScreen::MenuFrame() {
#ifdef _WIN32
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Play Game")) {
		command = ScreenCommand::TransitionToNextScreen;
	}
	if (ImGui::Button("Exit")) {
		command = ScreenCommand::Exit;
	}
	ImGui::End();
#endif
}

PushdownState::PushdownResult SplashScreen::onStateChange(PushdownState** newState) {
	switch (command)
	{
		case ScreenCommand::TransitionToNextScreen: 
			*newState = screenManager->GetScreen(ScreenType::MainMenuScreen);
			return PushdownResult::Push;
		case ScreenCommand::Exit: 
			return PushdownResult::Pop;
		default: 
			return PushdownResult::NoChange;
	}
}

void SplashScreen::TransitionTimer(float dt) {
#ifdef __ORBIS__
	totalTime += dt;

	if (totalTime > 3.0f) {
		command = ScreenCommand::TransitionToNextScreen;
	}
#endif
}
