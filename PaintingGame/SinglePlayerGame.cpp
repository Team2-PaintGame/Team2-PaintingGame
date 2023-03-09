#include "SinglePlayerGame.h"
#include "GameManager.h"
#include "PlayerControllers.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameAssets * assets) : PaintingGame(assets) {
	if (!GameManager::sConfig.playerControllerFactory) {
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
	}
	AddPlayer(Vector3(20.0f, 10.0f, 50.0f), Team::Blue);
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

void SinglePlayerGame::SplatterOnPlayerCamera() {
	if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::SPACE)) {
		HudSplatter* splatter = CreateSplatter(Team::Blue);
		world->AddGameObject(splatter);
	}
}

Player* SinglePlayerGame::AddPlayer(Vector3 position, Team team) {
	player = CreatePlayer(position, team);
	activeCameras.push_back(player->GetCamera());
	world->AddGameObject(player);
	playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(player);
	
	FocusPoint* focusPoint = CreateFocusPoint();
	focusPoint->SetPlayer(player);
	world->AddGameObject(focusPoint);

	GameObject* gun = CreateGun(player->GetTransform().GetPosition(), team);
	player->SetGun(gun);
	world->AddGameObject(gun);
	return player;
}

void SinglePlayerGame::Update(float dt) {
	playerController->Update(dt);
	PaintingGame::Update(dt);
}