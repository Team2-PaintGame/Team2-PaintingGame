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
	AddPlayer(Vector3(0.0f, 10.0f, 0.0f));

	//InitCamera(*this->world->GetMainCamera(), *player);
}

SinglePlayerGame::~SinglePlayerGame() {
	
}

Player* SinglePlayerGame::AddPlayer(Vector3 position)
{
	player = CreatePlayer(position);
	world->AddGameObject(player);
	playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(player);

	return player;
}

void SinglePlayerGame::Update(float dt) {
	playerController->Update(dt);
	//world->GetMainCamera()->UpdateCamera(dt);
	//player->GetCamera()->UpdateCamera(dt);
	PaintingGame::Update(dt);
}
