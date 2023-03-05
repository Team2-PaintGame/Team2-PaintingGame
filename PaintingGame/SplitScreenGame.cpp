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
	InitWorld();
	InitPlayers();
	//InitCamera(*this->world->GetMainCamera(), *players[0], 0.5f);
	//InitCamera(*this->world->GetSecondCamera(), *players[1], 0.5f);
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

PlayerBase* SplitScreenGame::AddPlayer(Vector3 position) {
	PlayerBase* player = CreatePlayer(position);
	players.push_back(player);
	world->AddGameObject(player);
	//playerControllers.push_back(new PlayerController(camera, player, gamepad));
	GameManager::sConfig.playerControllerFactory->createPlayerController(player);
	return player;
}

void SplitScreenGame::Update(float dt) {

	for (PlayerController* pc : playerControllers)
	{
		pc->Update(dt);
	}

	//world->GetMainCamera()->UpdateCamera(dt);
	//world->GetSecondCamera()->UpdateCamera(dt);

	PaintingGame::Update(dt);
}


