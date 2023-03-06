#include "GameScreen.h"
#include "SinglePlayerGame.h"
#include "SplitScreenGame.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8508;

void GameScreen::OnAwake() {
	isMenuDisplayed = false;
	LoadGame();
}

void GameScreen::LoadGame() {
	switch (command)
	{
	case ScreenCommand::CreateSinglePlayerGame: 
		sceneNode = new SinglePlayerGame(screenManager->GetGameAssets());
	break;
	case ScreenCommand::CreateSplitScreenGame:
		sceneNode = new SplitScreenGame(screenManager->GetGameAssets());
		break;
	case ScreenCommand::CreateNetworkedGameAsServer:
		sceneNode = new NetworkedGame(screenManager->GetGameAssets());
		((NetworkedGame*)(sceneNode))->StartAsServer();
		break;
	case ScreenCommand::CreateNetworkedGameAsClient:
		sceneNode = new NetworkedGame(screenManager->GetGameAssets());
		((NetworkedGame*)(sceneNode))->StartAsClient(127, 0, 0, 1);
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
