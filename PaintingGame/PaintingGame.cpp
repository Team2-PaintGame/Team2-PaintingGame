#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "Assets.h"
#include "Wall.h"
#include "../GameObjects/MuseumItem.h"
#include "SecurityGuard.h"
//#include "InputController.h"
#include "AnimationController.h"
#include "PaintingObject.h"
#include "Ink.h"
#include "EventListener.h"
#include "GameTimer.h"
#include "GameScreen.h"


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
	world->AddEventListener(new GameEventListener(&world->GetPhysicsWorld(), world));
	timer = GameTimer();
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

	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(187, 10,1.2),Quaternion(), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("monaLisaMat"), assets->GetShader("basicShader"), 10, "MonaLisa"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(248, 10, 53), Quaternion::EulerAnglesToQuaternion(0,90,180), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("handsPaintingMat"), assets->GetShader("basicShader"), 10, "handsPaint"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(248, 10, 144), Quaternion::EulerAnglesToQuaternion(0, 90, 180), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("nightSkyMat"), assets->GetShader("basicShader"), 10, "nightSky"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(103.6, 10, 46), Quaternion::EulerAnglesToQuaternion(0, -90, 180), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("screamPaintMat"), assets->GetShader("basicShader"), 10, "screamPaint"));
	world->AddGameObject(new PaintingObject(physicsCommon, physicsWorld, Vector3(65.5, 10, 248), Quaternion::EulerAnglesToQuaternion(0, 0, 180), assets->GetMesh("cubeMesh"), assets->GetMeshMaterial("sunflowersMat"), assets->GetShader("basicShader"), 10, "sunflowers"));

}

void PaintingGame::Update(float dt) {
	Debug::DrawFPS();
	Debug::ShowMemoryUsage();
	Debug::ShowNumberOfGameObjects(world->GetNumberOfGameObjects());
	Debug::ShowNumberOfPaintedPositions(world->GetNumPaintedPositions());
	Debug::ShowNumberOfParticals(ink->GetParticalsSize());

	world->UpdateWorld(dt);
	world->CalculateNewScores();
	physicsWorld->update(dt);
	gameTime -= dt;
	Debug::Print(std::to_string((int)gameTime), Vector2(20, 5));
}

Player* PaintingGame::CreatePlayer(NCL::Maths::Vector3 position,Team team, bool networked) {
	std::unordered_map<std::string, MeshAnimation*> animations;
	animations.insert(std::make_pair("idleAnimation", assets->GetMeshAnimation("mainCharIdleAnim")));
	animations.insert(std::make_pair("moveAnimation", assets->GetMeshAnimation("mainCharRunAnim")));
	Player* player;
	if (team == Team::Red) {
		player =  new Player(physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetMeshMaterial("redMainCharMat"), assets->GetShader("skinningShader"), animations, 5, 0, CreateGun(position, team), "Red Player", networked);
	}
	else { //blue
		player =  new Player(physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetMeshMaterial("blueMainCharMat"), assets->GetShader("SecondskinningShader"), animations, 5, 1, CreateGun(position, team), "Blue Player", networked);
	}

	world->AddGameObject(player);
	return player;
}

Gun* PaintingGame::CreateGun(NCL::Maths::Vector3 position, Team team) {
	Gun* gun;
	
	FocusPoint* reticle = new FocusPoint(physicsCommon, physicsWorld, assets->GetMesh("quadMesh"), assets->GetTexture("gunFocusTex"), assets->GetShader("hudShader"), Vector2(0.05));

	if (team == Team::Red) {
		ink = CreateInkStream(physicsCommon, physicsWorld, Vector3(0, 10, 0), assets->GetMesh("sphereMesh"), Debug::RED, assets->GetShader("inkShader"));
		gun = new Gun(physicsCommon, physicsWorld, position, assets->GetMesh("gunMesh"), assets->GetMeshMaterial("gunMat"), assets->GetShader("basicShader"), 3, Debug::RED, ink, reticle);
	}
	else { //blue
		ink = CreateInkStream(physicsCommon, physicsWorld, Vector3(0, 10, 0), assets->GetMesh("sphereMesh"), Debug::BLUE, assets->GetShader("inkShader"));
		gun = new Gun(physicsCommon, physicsWorld, position, assets->GetMesh("gunMesh"), assets->GetMeshMaterial("gunMat"), assets->GetShader("basicShader"), 3, Debug::BLUE, ink, reticle);
	}

	world->AddGameObject(gun);
	world->AddGameObject(ink);
	world->AddGameObject(reticle);
	return gun;
}

void PaintingGame::AddSecurityAI(NCL::CSC8508::Vector3 position, PlayerBase* target1, PlayerBase* target2) // Vector3(-70.0f, 5.0f, 60.0f) // Change the hardcoded two targets
{
	std::unordered_map<std::string, MeshAnimation*> animations;
	animations.insert(std::make_pair("idleAnimation", assets->GetMeshAnimation("AiIdleAnim")));
	animations.insert(std::make_pair("moveAnimation", assets->GetMeshAnimation("AiRunAnim")));

	world->AddGameObject(new SecurityGuard(physicsCommon, physicsWorld, position, assets->GetMesh("AiMesh"), assets->GetMeshMaterial("AiMat"), assets->GetShader("THIRDskinningShader"), animations, 4, target1, target2, world, assets, "Security Guard"));
	
}

