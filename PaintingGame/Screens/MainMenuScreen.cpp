#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

void MainMenuScreen::MenuFrame() {
	ImGui::Begin("Main Menu");
	if (ImGui::Button("Single Player Game")) {
		command = ScreenCommand::CreateSinglePlayerGame;
	}
	if (ImGui::Button("Split Screen: 2 Player Game")) {
		command = ScreenCommand::CreateSplitScreenGame;
	}
	if (ImGui::Button("LAN Game")) {
		command = ScreenCommand::CreateNetworkedGame;
	}	
	if (ImGui::Button("Exit")) {
		command = ScreenCommand::TransitionToPreviousScreen;
	}
	ImGui::End();
}

PushdownState::PushdownResult MainMenuScreen::onStateChange(PushdownState** newState) {
	if (command == ScreenCommand::CreateSinglePlayerGame || command == ScreenCommand::CreateSplitScreenGame || command == ScreenCommand::CreateNetworkedGame) {
		*newState = screenManager->GetScreen(ScreenType::GameScreen);
		((BaseScreen*)(*newState))->SetCommand(command);
		return PushdownResult::Push;
	}
	return PushdownResult::NoChange;
}
