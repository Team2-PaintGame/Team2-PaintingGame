#include "GameScreen.h"
#include "SinglePlayerGame.h"
#include "SplitScreenGame.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8508;

bool GameScreen::sPauseCallback = false;

void GameScreen::OnAwake() {
	isMenuDisplayed = false;
	LoadGame();
	sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
}

void GameScreen::LoadGame() {

	delete sceneNode;

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

PushdownState::PushdownResult GameScreen::OnUpdate(float dt, PushdownState** newState) {
	if (sPauseCallback) {
		isMenuDisplayed = true;
		sPauseCallback = false;
	}
	return BaseScreen::OnUpdate(dt, newState);
}

void GameScreen::MenuFrame() {
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Resume")) {
		isMenuDisplayed = false;
	}
	if (ImGui::Button("Toggle Debug Lines")) {
		isDebugRenderingEnabled = !isDebugRenderingEnabled;
		sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
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

void GameScreen::GamePauseCallback() {
	sPauseCallback = true;
}
