#include "SplitScreenGame.h"
#include "GameManager.h"
#include "PlayerControllers.h"

using namespace NCL;
using namespace CSC8508;

SplitScreenGame::SplitScreenGame(GameAssets* assets) : PaintingGame(assets) {
	
	players.reserve(maxPlayers);
	if (!GameManager::sConfig.playerControllerFactory) {
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
		secondPlayerControllerFactory = new XBoxPlayerControllerFactory();
	}
 	else if (GameManager::sConfig.playerControllerFactory->GetType() != PlayerControllerFactory::Type::PS4) {
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
		secondPlayerControllerFactory = new XBoxPlayerControllerFactory();
	}
	InitPlayers();
}

SplitScreenGame::~SplitScreenGame() {
	for (auto p : playerControllers)
	{
		delete p;
	}
	delete secondPlayerControllerFactory;
	playerControllers.clear();
}

void SplitScreenGame::InitPlayers() {
	players.clear();
	
	Player* player1 = AddPlayer(Vector3(20.0f, 10.0f, 20.0f), Team::Red);
	Player* player2 = AddPlayer(Vector3(30.0f, 10.0f, 20.0f), Team::Blue);

	playerControllers.push_back(GameManager::sConfig.playerControllerFactory->createPlayerController(player1));
	
	//if second player controller is not set, use the factory from GameManager::sConfig to create the second player controller
	if (secondPlayerControllerFactory) {
		playerControllers.push_back(secondPlayerControllerFactory->createPlayerController(player2));
	}
	else {
		playerControllers.push_back(GameManager::sConfig.playerControllerFactory->createPlayerController(player2));
	}

	player1->GetCamera()->SetVpSize(0.5f);
	player2->GetCamera()->SetVpSize(0.5f);
	player1->GetCamera()->SetVpStartPos(Vector2(0.0f, 0.0f));
	player2->GetCamera()->SetVpStartPos(Vector2(0.5f, 0.0f));

	AddSecurityAI(Vector3(100, 5, 100), player1, player2);
}

void SplitScreenGame::CreateSplatOnShoot() {
	int index = 0;
	for (const auto& player : players) {
		if (playerControllers[index]->Shoot()) {
			SceneContactPoint* closestCollision = world->Raycast(player->GetShootRay());
			if (closestCollision->isHit) {
				world->AddPaintedPosition(closestCollision->hitPos);
			}
		}
		index++;
	}
}

Player* SplitScreenGame::AddPlayer(Vector3 position,Team team) {
	Player* player = CreatePlayer(position, team);
	activeCameras.push_back(player->GetCamera());
	players.push_back(player);
	return player;
}

void SplitScreenGame::Update(float dt) {

	for (PlayerController* pc : playerControllers) {
		pc->Update(dt);
	}
	PaintingGame::Update(dt);
}


