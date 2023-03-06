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
		command = ScreenCommand::Exit;
	}
	ImGui::End();
}

PushdownState::PushdownResult MainMenuScreen::onStateChange(PushdownState** newState) {
	switch (command) {
		case ScreenCommand::CreateSinglePlayerGame: {
			*newState = screenManager->GetScreen(ScreenType::GameScreen);
			return PushdownResult::Push;
		}
		case ScreenCommand::CreateSplitScreenGame: {
			*newState = screenManager->GetScreen(ScreenType::GameScreen);
			return PushdownResult::Push;
		}
		case ScreenCommand::CreateNetworkedGame: {
			*newState = screenManager->GetScreen(ScreenType::GameScreen);
			return PushdownResult::Push;
		}
		case ScreenCommand::Exit: {
			return PushdownResult::Pop;
		}
		default: {
			return PushdownResult::NoChange;
		}
	}
}
