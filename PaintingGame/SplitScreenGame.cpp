#include "SplitScreenGame.h"
#include "GameManager.h"
#include "XBoxGamepad.h"

using namespace NCL;
using namespace CSC8508;

SplitScreenGame::SplitScreenGame(GameAssets* assets) : PaintingGame(assets) {
	
	players.reserve(maxPlayers);
	//change this through settings obj
	//render->SetRenderMode(GameTechRenderer::RenderMode::SplitScreen);
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
	
	AddPlayer(Vector3(0.0f, 10.0f, 0.0f));
	AddPlayer(Vector3(20.0f, 10.0f, 20.0f));
}

Player* SplitScreenGame::AddPlayer(Vector3 position) {
	Player* player = CreatePlayer(position);
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


