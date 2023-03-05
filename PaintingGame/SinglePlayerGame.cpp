#include "SinglePlayerGame.h"
#include "GameManager.h"
#include "Win32Window.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameAssets* assets) : PaintingGame(assets) {
	//change this through settings obj
	//renderer->SetRenderMode(GameTechRenderer::RenderMode::SingleViewport);
	
	if (!GameManager::sConfig.playerControllerFactory) {
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
	}
	InitWorld();
	AddPlayer(world->GetMainCamera(), Vector3(0.0f, 10.0f, 0.0f), nullptr);

	//InitCamera(*this->world->GetMainCamera(), *player);
}

SinglePlayerGame::~SinglePlayerGame() {
	
}

PlayerBase* SinglePlayerGame::AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad)
{
	player = CreatePlayer(position);
	world->AddGameObject(player);
	playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(player);

	return player;
}

void SinglePlayerGame::Update(float dt) {
	playerController->Update(dt);
	world->GetMainCamera()->UpdateCamera(dt);

	PaintingGame::Update(dt);
}
