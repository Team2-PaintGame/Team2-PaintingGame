#include "GameScreen.h"
#include "SinglePlayerGame.h"
#include "SplitScreenGame.h"
#include "NetworkedGame.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8508;

bool GameScreen::sPauseCallback = false;

void GameScreen::OnAwake() {
	isMenuDisplayed = false;
	isDebugDisplayed = false;
	LoadGame();
	sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);

	Window::GetWindow()->LockMouseToWindow(true);
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
	if (ImGui::Button("Debug Window")) {
		isDebugDisplayed = true;
		isMenuDisplayed = false;
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

void NCL::CSC8508::GameScreen::DebugWindow()
{
	ImGui::Begin("Debug Window");
	ImGui::Text(std::to_string(Debug::fps).c_str());
	ImGui::Text("Number of GameObjects");
	ImGui::Text(std::to_string(Debug::numberOfGameObjects).c_str());
	ImGui::Text("Number of Paints");
	ImGui::Text(std::to_string(Debug::numberOfPaints).c_str());
	ImGui::Text("Number of Particals");
	ImGui::Text(std::to_string(Debug::numberOfParticals).c_str());
	if (ImGui::Button("Memory Footprint")) {
		ImGui::OpenPopup("MemoryFootprint");
	}
	if(ImGui::Button("Exit")) {
		isDebugDisplayed = false;
	}
	if (ImGui::BeginPopupModal("MemoryFootprint")) {

		ImGui::Text(std::to_string(Debug::PageFaultCount).c_str());
		ImGui::Text(std::to_string(Debug::PeakWorkingSetSize).c_str());
		ImGui::Text(std::to_string(Debug::WorkingSetSize).c_str());
		ImGui::Text(std::to_string(Debug::QuotaNonPagedPoolUsage).c_str());
		ImGui::Text(std::to_string(Debug::QuotaPagedPoolUsage).c_str());
		ImGui::Text(std::to_string(Debug::QuotaPeakNonPagedPoolUsage).c_str());
		ImGui::Text(std::to_string(Debug::QuotaPeakPagedPoolUsage).c_str());

		if (ImGui::Button("Exit")) {
			isDebugDisplayed = false;
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

void GameScreen::GamePauseCallback() {
	sPauseCallback = true;
}
