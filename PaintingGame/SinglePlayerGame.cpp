#include "SinglePlayerGame.h"
#include "GameManager.h"
#include "PlayerControllers.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameAssets * assets) : PaintingGame(assets) {
#ifdef _WIN32
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
#endif
	AddPlayer(Vector3(13.0f, 4.2, 34.0f), Team::Blue);
	AddSecurityAI(Vector3(100, 5, 100), player, nullptr, seed);
}

SinglePlayerGame::~SinglePlayerGame() {
	delete playerController;
}

void SinglePlayerGame::CreateSplatOnShoot() {
	//if (playerController->Shoot()) {
	//	SceneContactPoint* closestCollision = world->Raycast(player->GetShootRay());
	//	if (closestCollision->isHit) {
			//world->AddPaintedPosition(closestCollision->hitPos, player->GetTeamColour());
			//std::cout << "X: " << closestCollision->hitPos.x << "Y: " << closestCollision->hitPos.y << "Z: " << closestCollision->hitPos.z << "\n";
	//	}
	//}
}

Player* SinglePlayerGame::AddPlayer(Vector3 position, Team team) {
	player = CreatePlayer(position, team);
	activeCameras.push_back(player->GetCamera());
	playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(player);
	return player;
}

void SinglePlayerGame::Update(float dt) {
	playerController->Update(dt);
	Window::GetWindow()->LockMouseToWindow(true);
	CreateSplatOnShoot();
	PaintingGame::Update(dt);
	Debug::Print("Team 1 score:" + std::to_string(world->GetTeamOneScore()), Vector2(5, 90));
}