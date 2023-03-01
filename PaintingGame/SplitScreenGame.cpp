#include "SplitScreenGame.h"
#include "InputController.h"

using namespace NCL;
using namespace CSC8508;

SplitScreenGame::SplitScreenGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu) : PaintingGame(render, world, physicsCommon, menu) {
	
	players.reserve(maxPlayers);
	//change this through settings obj
	render->SetRenderMode(GameTechRenderer::RenderMode::SplitScreen);

	gamepad = new Gamepad();

	InitWorld();
	InitPlayers();
	InitCamera(*this->world->GetMainCamera(), *players[0], 0.5f);
	InitCamera(*this->world->GetSecondCamera(), *players[1], 0.5f);
}

SplitScreenGame::~SplitScreenGame() {
	for (auto p : playerControllers)
	{
		delete p;
	}
	playerControllers.clear();

	delete gamepad;
}

void SplitScreenGame::InitPlayers() {
	players.clear();
	
	AddPlayer(world->GetMainCamera(), Vector3(0.0f, 10.0f, 0.0f));
	AddPlayer(world->GetSecondCamera(), Vector3(20.0f, 10.0f, 20.0f), gamepad);
}

PlayerBase* SplitScreenGame::AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad) {
	PlayerBase* player = CreatePlayer(position);
	players.push_back(player);
	world->AddGameObject(player);
	playerControllers.push_back(new PlayerController(camera, player, gamepad));

	return player;
}

void SplitScreenGame::UpdateGame(float dt) {

	for (PlayerController* pc : playerControllers)
	{
		pc->Update(dt);
	}

	world->GetMainCamera()->UpdateCamera(dt);
	world->GetSecondCamera()->UpdateCamera(dt);

	PaintingGame::UpdateGame(dt);
}


