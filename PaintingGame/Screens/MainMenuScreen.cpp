#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

void MainMenuScreen::MenuFrame() {
#ifdef _WIN32
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
#endif
}

PushdownState::PushdownResult MainMenuScreen::onStateChange(PushdownState** newState) {
	switch (command)
	{
	case ScreenCommand::CreateSinglePlayerGame:
	case ScreenCommand::CreateSplitScreenGame:
	case ScreenCommand::CreateNetworkedGameAsServer:
	case ScreenCommand::CreateNetworkedGameAsClient:
	{
		*newState = screenManager->GetScreen(ScreenType::GameScreen);
		((BaseScreen*)(*newState))->SetCommand(command);
		return PushdownResult::Push;
	}
	case  ScreenCommand::TransitionToPreviousScreen:
	{
		return PushdownResult::Pop;
	}
	default:
	{
		return PushdownResult::NoChange;
	}
	}
}
void MainMenuScreen::TransitionTimer(float dt) {
#ifdef __ORBIS__
	totalTime += dt;

	if (totalTime > 3.0f) {
		command = ScreenCommand::CreateSinglePlayerGame;
	}
#endif
}
