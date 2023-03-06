#include "SplitScreenGame.h"
#include "GameManager.h"
#include "XBoxGamepad.h"

using namespace NCL;
using namespace CSC8508;

SplitScreenGame::SplitScreenGame(GameAssets* assets) : PaintingGame(assets) {
	
	players.reserve(maxPlayers);
	if (!GameManager::sConfig.playerControllerFactory) {
		GameManager::sConfig.playerControllerFactory = new XBoxPlayerControllerFactory();
	}
	InitPlayers();
}

SplitScreenGame::~SplitScreenGame() {
	for (auto p : playerControllers)
	{
		delete p;
	}
	playerControllers.clear();
}

void SplitScreenGame::InitPlayers() {
	players.clear();
	
	Player* player1 = AddPlayer(Vector3(0.0f, 10.0f, 0.0f));
	Player* player2 = AddPlayer(Vector3(20.0f, 10.0f, 20.0f));

	player1->GetCamera()->SetViewportDivider(0.5f);
	player2->GetCamera()->SetViewportDivider(0.5f);
	player1->GetCamera()->SetViewportSize(Vector2(0.0f, 0.0f));
	player2->GetCamera()->SetViewportSize(Vector2(0.5f, 0.0f));
}

Player* SplitScreenGame::AddPlayer(Vector3 position) {
	Player* player = CreatePlayer(position);
	activeCameras.push_back(player->GetCamera());
	players.push_back(player);
	world->AddGameObject(player);
	playerControllers.push_back(GameManager::sConfig.playerControllerFactory->createPlayerController(player));
	return player;
}

void SplitScreenGame::Update(float dt) {

	for (PlayerController* pc : playerControllers) {
		pc->Update(dt);
	}
	PaintingGame::Update(dt);
}


