#include "GameScreen.h"
#include "SinglePlayerGame.h"
#include "SplitScreenGame.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8508;

void GameScreen::OnAwake() {
	isMenuDisplayed = false;
}

void GameScreen::LoadGame() {
	switch (command)
	{
	case ScreenCommand::CreateSinglePlayerGame: 
		sceneNode->AddChild(new SinglePlayerGame());
	break;
	case ScreenCommand::CreateSplitScreenGame:
		sceneNode->AddChild(new SinglePlayerGame());
		break;
	case ScreenCommand::CreateNetworkedGame: 
		sceneNode->AddChild(new SinglePlayerGame());
		break;
	default:
		std::cout << "No instance of game could be created: No appropriate Command Selected." << std::endl;
	}
}

void GameScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Pause")) {
		isMenuDisplayed = true;
	}
	if (ImGui::Button("Quit Game")) {
		command = ScreenCommand::TransitionToPreviousScreen;
	}
	ImGui::End();
}

PushdownState::PushdownResult GameScreen::onStateChange(PushdownState** newState) {
	//add game-over game-win checks here:
	switch (command) {
		case ScreenCommand::TransitionToPreviousScreen:
			return PushdownResult::Pop;
		default:
			return PushdownResult::NoChange;
	}
}
