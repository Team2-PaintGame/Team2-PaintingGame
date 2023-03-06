#include "SinglePlayerGame.h"
#include "InputController.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu) : PaintingGame(render, world, physicsCommon, menu) {
	//change this through settings obj
	render->SetRenderMode(GameTechRenderer::RenderMode::SingleViewport);

	gamepad = new Gamepad();

	InitWorld();
	AddPlayer(world->GetMainCamera(), Vector3(10.0f, 30.0f, 10.0f), nullptr);
	InitCamera(*this->world->GetMainCamera(), *player);
	AddSecurityAI(Vector3(0,0,0), player, nullptr);
}

SinglePlayerGame::~SinglePlayerGame() {
	delete playerController;
	delete gamepad;
}

PlayerBase* SinglePlayerGame::AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad)
{
	player = CreatePlayer(position);
	playerController = new PlayerController(world->GetMainCamera(), player, gamepad);
	world->AddGameObject(player);
	return player;
}

void SinglePlayerGame::UpdateGame(float dt) {
	playerController->Update(dt);
	world->GetMainCamera()->UpdateCamera(dt);

	PaintingGame::UpdateGame(dt);
}
