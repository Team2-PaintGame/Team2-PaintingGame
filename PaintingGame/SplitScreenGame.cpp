#include "SplitScreenGame.h"
#include "GameManager.h"
#include "PlayerControllers.h"

using namespace NCL;
using namespace CSC8508;

SplitScreenGame::SplitScreenGame(GameAssets* assets) : PaintingGame(assets) {
	
	players.reserve(maxPlayers);
#ifdef _WIN32
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
		secondPlayerControllerFactory = new XBoxPlayerControllerFactory();
#endif
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
	
	Player* player1 = AddPlayer(Vector3(13.0f, 4.2, 34.0f), Team::Blue);
	Player* player2 = AddPlayer(Vector3(194.0f, 4.24f, 208.0f), Team::Red);

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

	AddSecurityAI(Vector3(100, 5, 100), player1, player2, seed);
}

void SplitScreenGame::CreateSplatOnShoot() {
	int index = 0;
	for (const auto& player : players) {
		if (playerControllers[index]->Shoot()) {
			SceneContactPoint* closestCollision = world->Raycast(player->GetShootRay());
			if (closestCollision->isHit) {
				//world->AddPaintedPosition(closestCollision->hitPos, player->GetTeamColour());
				//std::cout << "X: " << closestCollision->hitPos.x << "Y: " << closestCollision->hitPos.y << "Z: " << closestCollision->hitPos.z << "\n";
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
	Window::GetWindow()->LockMouseToWindow(true);
	Debug::Print("Blue Team Score:" + std::to_string(world->GetTeamOneScore()), Vector2(5, 90));
	Debug::Print("Red Team Score:" + std::to_string(world->GetTeamTwoScore()), Vector2(60, 90));
}


