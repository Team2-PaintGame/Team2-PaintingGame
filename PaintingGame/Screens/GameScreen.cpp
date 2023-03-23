#include "GameScreen.h"
#include "SinglePlayerGame.h"
#include "SplitScreenGame.h"
#include "GameOverScreen.h"
#ifdef _WIN32
#include "NetworkedGame.h"
#endif
#include "Window.h"

using namespace NCL;
using namespace CSC8508;

bool GameScreen::sPauseCallback = false;

void NCL::CSC8508::GameScreen::OnAwake()
{
	isMenuDisplayed = false;
	isDebugDisplayed = false;
	LoadGame(screenManager->GetGameAssets());

	if (sceneNode)
	{
		sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
		sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		sceneNode->GetPhysicsWorld()->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);

		Window::GetWindow()->LockMouseToWindow(true);
	}
}

void NCL::CSC8508::GameScreen::OnSleep()
{
	delete sceneNode;
	sceneNode = nullptr;
}


void GameScreen::LoadGame(GameAssets* assets) {

	delete sceneNode;
	switch (command)
	{
	case ScreenCommand::CreateSinglePlayerGame:
		sceneNode = new SinglePlayerGame(assets);
		bisNetworkedGame = false;
		break;
	case ScreenCommand::CreateSplitScreenGame:
		sceneNode = new SplitScreenGame(assets);
		bisNetworkedGame = false;
		break;
#ifdef _WIN32
	case ScreenCommand::CreateNetworkedGameAsServer:
		sceneNode = new NetworkedGame(assets);
		((NetworkedGame*)(sceneNode))->StartAsServer();
		bisNetworkedGame = true;
		break;
	case ScreenCommand::CreateNetworkedGameAsClient:
		sceneNode = new NetworkedGame(assets);
		((NetworkedGame*)(sceneNode))->StartAsClient(127, 0, 0, 1);
		bisNetworkedGame = true;
		break;
#endif
	default:
		std::cout << "No instance of game could be created: No appropriate Command Selected." << std::endl;
	}
}

PushdownState::PushdownResult GameScreen::OnUpdate(float dt, PushdownState** newState) {
	if (sPauseCallback) {
		isMenuDisplayed = true;
		sPauseCallback = false;
	}

	if (sceneNode && ((PaintingGame*)sceneNode)->GetTimeOver())
	{
		command = ScreenCommand::TransitionToNextScreen;
	}


	return BaseScreen::OnUpdate(dt, newState);
}

void GameScreen::MenuFrame() {
#ifdef _WIN32
	ImGui::Begin("Painting Game");
	if (ImGui::Button("Resume")) {
		isMenuDisplayed = false;
	}
	if (ImGui::Button("Toggle Debug Lines")) {
		isDebugRenderingEnabled = !isDebugRenderingEnabled;
		sceneNode->GetPhysicsWorld()->setIsDebugRenderingEnabled(isDebugRenderingEnabled);
		isMenuDisplayed = false;
	}
	if (ImGui::Button("Debug Window")) {
		isDebugDisplayed = true;
		isMenuDisplayed = false;
	}
	if (ImGui::Button("Quit to Main Menu")) {
		command = ScreenCommand::TransitionToPreviousScreen;
	}
	if (ImGui::Button("Quit Game")) {
		command = ScreenCommand::Exit;
	}
	ImGui::End();
#endif
}

PushdownState::PushdownResult GameScreen::onStateChange(PushdownState** newState) {
	//add game-over game-win checks here:
	switch (command) {
		case ScreenCommand::TransitionToNextScreen:
			*newState = screenManager->GetScreen(ScreenType::GameOverScreen);
			((GameOverScreen*)*newState)->SetWinner(((PaintingGame*)sceneNode)->GetWinner());
			return PushdownResult::Push;
		case ScreenCommand::TransitionToPreviousScreen:
			return PushdownResult::Pop;
		case ScreenCommand::Exit:
			return PushdownResult::Reset1;
		default:
			return PushdownResult::NoChange;
	}
}

void NCL::CSC8508::GameScreen::DebugWindow()
{
#ifdef _WIN32
	ImGui::Begin("Debug Window");
	ImGui::Text(("FPS: " + std::to_string(Debug::fps)).c_str());
	ImGui::Text(("Rendertime: " + std::to_string(Debug::renderingTime) + " us").c_str());
	ImGui::Text(("Number of GameObjects: " + std::to_string(Debug::numberOfGameObjects)).c_str());
	ImGui::Text(("Number of Paints: " + std::to_string(Debug::numberOfPaints)).c_str());
	ImGui::Text(("Number of Particles: " + std::to_string(Debug::numberOfParticals)).c_str());
	if (ImGui::Button("Memory Footprint")) {
		ImGui::OpenPopup("MemoryFootprint");
	}
	if(ImGui::Button("Exit")) {
		isDebugDisplayed = false;
	}
	if (ImGui::BeginPopupModal("MemoryFootprint")) {
		ImGui::Text(("totalVirtualMemory: " + std::to_string(Debug::totalVirtualMemory / 1073741824) + "GB").c_str());
		ImGui::Text(("usedVirtualMemory: " + std::to_string(Debug::usedVirtualMemory / 1073741824) + "GB").c_str());
		ImGui::Text(("totalPhysMemory: " + std::to_string(Debug::totalPhysMemory / 1073741824) + "GB").c_str());
		ImGui::Text(("usedphysMemory: " + std::to_string(Debug::usedphysMemory / 1073741824) + "GB").c_str());

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
#endif
}

void GameScreen::GamePauseCallback() {
	sPauseCallback = true;
}
