#include "SinglePlayerGame.h"
#include "InputController.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameAssets* assets) : PaintingGame(assets) {
	//change this through settings obj
	//renderer->SetRenderMode(GameTechRenderer::RenderMode::SingleViewport);

	gamepad = new Gamepad();

	InitWorld();
	AddPlayer(world->GetMainCamera(), Vector3(0.0f, 10.0f, 0.0f), nullptr);
	//InitCamera(*this->world->GetMainCamera(), *player);
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

void SinglePlayerGame::Update(float dt) {
	playerController->Update(dt);
	world->GetMainCamera()->UpdateCamera(dt);

	PaintingGame::Update(dt);
}
