#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "AnimationController.h"
#include "PaintingObject.h"
#include "Ink.h"

using namespace NCL;
using namespace CSC8508;

PaintingGame::PaintingGame(GameAssets* assets) {
	this->assets = assets;
	physicsWorld = physicsCommon.createPhysicsWorld();
	physicsWorld->setIsGravityEnabled(useGravity);
	world = new GameWorld(physicsWorld);
	InitWorld();
	//renderer->settings.SetIsDebugRenderingModeEnabled(true);
	//renderer->settings.debugRendererSettings.SetIsCollisionShapeDisplayed(true);
	//renderer->settings.debugRendererSettings.SetIsBroadPhaseAABBDisplayed(true);
}

PaintingGame::~PaintingGame() {
	delete world;
	physicsCommon.destroyPhysicsWorld(physicsWorld);
}

void PaintingGame::OperateOnCameras(CameraFunc f) {
	for (Camera* cam : activeCameras) {
		f(cam);
	}
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();

	world->AddGameObject(new Floor(physicsCommon, physicsWorld, Vector3(0, 0, 0), assets->GetMesh("floorMesh"), assets->GetTexture("basicTex"), assets->GetShader("basicShader"), 1));

	for (int x = 0; x < 15; ++x) {
		world->AddGameObject(new Box(physicsCommon, physicsWorld, Vector3(0, 10, 0), assets->GetMesh("cubeMesh"), assets->GetTexture("doorTex"), assets->GetShader("basicShader"), 2));
	}

	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(30, 10, 50), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("monaLisaMat"), assets->GetShader("basicShader"), 10, "MonaLisa"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(20, 10, 50), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("handsPaintingMat"), assets->GetShader("basicShader"), 10, "handsPaint"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(10, 10, 50), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("nightSkyMat"), assets->GetShader("basicShader"), 10, "nightSky"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(50, 10, 50), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("screamPaintMat"), assets->GetShader("basicShader"), 10, "screamPaint"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(60, 10, 50), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("sunflowersMat"), assets->GetShader("basicShader"), 10, "sunflowers"));
  
  world->AddGameObject(CreateInkStream<SphereParticle>(physicsCommon, physicsWorld, Vector3(0, 10, 0), assets->GetMesh("sphereMesh"), Vector4(0, 1, 0, 1), assets->GetShader("inkShader")));
	world->AddGameObject(CreateInkSplash<SphereParticle>(physicsCommon, physicsWorld, Vector3(0, 9.5, 0), assets->GetMesh("sphereMesh"), Vector4(0, 1, 0, 1), assets->GetShader("inkShader")));
}

void PaintingGame::Update(float dt) {
	world->UpdateWorld(dt);
	physicsWorld->update(dt);
	CreateSplatOnShoot();
	Debug::UpdateRenderables(dt);
}

Player* PaintingGame::CreatePlayer(Vector3 position,Team team) {
	std::unordered_map<std::string, MeshAnimation*> animations;
	animations.insert(std::make_pair("idleAnimation", assets->GetMeshAnimation("mainCharIdleAnim")));
	animations.insert(std::make_pair("moveAnimation", assets->GetMeshAnimation("mainCharRunAnim")));

	if (team == Team::Red) {
		return new Player(physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetMeshMaterial("redMainCharMat"), assets->GetShader("skinningShader"), animations, 5);

	}

	if (team == Team::Blue) {
		return new Player(physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetMeshMaterial("blueMainCharMat"), assets->GetShader("skinningShader"), animations, 5);

	}
}
