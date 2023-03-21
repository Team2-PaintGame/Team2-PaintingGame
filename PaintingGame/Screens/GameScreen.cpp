#include "GameScreen.h"
#include "SinglePlayerGame.h"
#include "SplitScreenGame.h"
#include "NetworkedGame.h"
#include "Window.h"

using namespace NCL;
using namespace CSC8508;

bool GameScreen::sPauseCallback = false;

void NCL::CSC8508::GameScreen::OnAwake()
{
	isMenuDisplayed = false;
	isDebugDisplayed = false;
	LoadGame(screenManager->GetGameAssets());
	sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);

	Window::GetWindow()->LockMouseToWindow(true);
}

void GameScreen::OnAwake1(GameAssets* assets) {
	//thread start 
	isMenuDisplayed = false;
	isDebugDisplayed = false;
	LoadGame(assets);
	sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);

	Window::GetWindow()->LockMouseToWindow(true);

}

void GameScreen::LoadGame(GameAssets* assets) {

	delete sceneNode;

	switch (command)
	{
	case ScreenCommand::CreateSinglePlayerGame: 
		sceneNode = new SinglePlayerGame(assets);
	break;
	case ScreenCommand::CreateSplitScreenGame:
		sceneNode = new SplitScreenGame(assets);
		break;
	case ScreenCommand::CreateNetworkedGameAsServer:
		sceneNode = new NetworkedGame(assets);
		((NetworkedGame*)(sceneNode))->StartAsServer();
		break;
	case ScreenCommand::CreateNetworkedGameAsClient:
		sceneNode = new NetworkedGame(assets);
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
	ImGui::Text(("FPS: " + std::to_string(Debug::fps)).c_str());
	ImGui::Text(("Rendertime: " + std::to_string(Debug::renderingTime) + " in us").c_str());
	ImGui::Text(("Number of GameObjects: " + std::to_string(Debug::numberOfGameObjects)).c_str());
	ImGui::Text(("Number of Paints: " + std::to_string(Debug::numberOfPaints)).c_str());
	ImGui::Text(("Number of Particals: " + std::to_string(Debug::numberOfParticals)).c_str());
	if (ImGui::Button("Memory Footprint")) {
		ImGui::OpenPopup("MemoryFootprint");
	}
	if(ImGui::Button("Exit")) {
		isDebugDisplayed = false;
	}
	if (ImGui::BeginPopupModal("MemoryFootprint")) {
		ImGui::Text(("PageFaultCount: " + std::to_string(Debug::PageFaultCount)).c_str());
		ImGui::Text(("PeakWorkingSetSize: " + std::to_string(Debug::PeakWorkingSetSize)).c_str());
		ImGui::Text(("WorkingSetSize: " + std::to_string(Debug::WorkingSetSize)).c_str());
		ImGui::Text(("QuotaNonPagedPoolUsage: " + std::to_string(Debug::QuotaNonPagedPoolUsage)).c_str());
		ImGui::Text(("QuotaPagedPoolUsage: " + std::to_string(Debug::QuotaPagedPoolUsage)).c_str());
		ImGui::Text(("QuotaPeakNonPagedPoolUsage: " + std::to_string(Debug::QuotaPeakNonPagedPoolUsage)).c_str());
		ImGui::Text(("QuotaPeakPagedPoolUsage: " + std::to_string(Debug::QuotaPeakPagedPoolUsage)).c_str());

		if (ImGui::Button("Exit")) {
			isDebugDisplayed = false;
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

void NCL::CSC8508::GameScreen::OnLoad(GameScreen* gs, GameAssets* assets)
{
	gs->OnAwake1(assets);
}

void GameScreen::GamePauseCallback() {
	sPauseCallback = true;
}
