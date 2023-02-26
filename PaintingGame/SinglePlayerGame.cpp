#include "SinglePlayerGame.h"

using namespace NCL;
using namespace CSC8508;

SinglePlayerGame::SinglePlayerGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu) : PaintingGame(render, world, physicsCommon, menu) {
	//change this through settings obj
	render->SetRenderMode(GameTechRenderer::RenderMode::SingleViewport);
}

SinglePlayerGame::~SinglePlayerGame() {
}

PlayerBase* SinglePlayerGame::AddPlayer() {
	player = new PlayerBase(*physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("mainChar"), textures.at("basicTex"), animController, shaders.at("skinningShader"), 5);
	world->AddGameObject(player);
	return player;
}
