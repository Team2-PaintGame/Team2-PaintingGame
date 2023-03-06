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
		ImGui::OpenPopup("Select Networked Game Mode");
	}	
	if (ImGui::Button("Exit")) {
		command = ScreenCommand::TransitionToPreviousScreen;
	}
	if (ImGui::BeginPopupModal("Select Networked Game Mode"))
	{
		if (ImGui::Button("Start As Server")) {
			command = ScreenCommand::CreateNetworkedGameAsServer;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Start As Client")) {
			command = ScreenCommand::CreateNetworkedGameAsClient;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

PushdownState::PushdownResult MainMenuScreen::onStateChange(PushdownState** newState) {
	if (command == ScreenCommand::CreateSinglePlayerGame || command == ScreenCommand::CreateSplitScreenGame || command == ScreenCommand::CreateNetworkedGameAsServer || command == ScreenCommand::CreateNetworkedGameAsClient) {
		*newState = screenManager->GetScreen(ScreenType::GameScreen);
		((BaseScreen*)(*newState))->SetCommand(command);
		return PushdownResult::Push;
	}
	return PushdownResult::NoChange;
}
