#include "SplitScreenGame.h"

using namespace NCL;
using namespace CSC8508;

SplitScreenGame::SplitScreenGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu) : PaintingGame(render, world, physicsCommon, menu) {
	players.reserve(maxPlayers);
	//change this through settings obj
	render->SetRenderMode(GameTechRenderer::RenderMode::SplitScreen);
}

SplitScreenGame::~SplitScreenGame() {
}

PlayerBase* SplitScreenGame::AddPlayer() {
	players.emplace_back(new PlayerBase(*physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("mainChar"), textures.at("basicTex"), animController, shaders.at("skinningShader"), 5));
	world->AddGameObject(players.back());
	return players.back();
}
