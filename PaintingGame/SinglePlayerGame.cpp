#include "SinglePlayerGame.h"
#include "GameManager.h"
#include "PlayerControllers.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameAssets * assets) : PaintingGame(assets) {
	if (!GameManager::sConfig.playerControllerFactory) {
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
	}
	AddPlayer(Vector3(120.0f, 5.0f, 120.0f), Team::Blue);
	AddSecurityAI(Vector3(100, 5, 100), player, nullptr);
}

SinglePlayerGame::~SinglePlayerGame() {
	delete playerController;
}

void SinglePlayerGame::CreateSplatOnShoot() {
	if (playerController->Shoot()) {
		SceneContactPoint* closestCollision = world->Raycast(player->GetShootRay());
		if (closestCollision->isHit) {
			world->AddPaintedPosition(closestCollision->hitPos);
		}
	}
}

Player* SinglePlayerGame::AddPlayer(Vector3 position, Team team) {
	player = CreatePlayer(position, team);
	activeCameras.push_back(player->GetCamera());
	playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(player);
	
	FocusPoint* focusPoint = CreateFocusPoint();
	focusPoint->SetPlayer(player);
	world->AddGameObject(focusPoint);

	return player;
}

void SinglePlayerGame::Update(float dt) {
	playerController->Update(dt);
	PaintingGame::Update(dt);
}