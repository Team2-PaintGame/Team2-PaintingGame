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
#include <glad/gl.h>


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

	maxSplats = 10000;

	glGenBuffers(1, &paintSplatSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, paintSplatSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, maxSplats * sizeof(PaintSplat), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, paintSplatSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	

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
	SendPaintSplatData();
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

void PaintingGame::SendPaintSplatData()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, paintSplatSSBO);

	for (int i = 0; i < world->GetNumPaintedPositions(); i++) {
		PaintSplat& splat = world->GetSplats()[i];
		if (!splat.sent) {
			int offset = i * sizeof(float) * 7;

			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(float), &(splat.position.x));
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + sizeof(float), sizeof(float), &(splat.position.y));
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 2 * sizeof(float), sizeof(float), &(splat.position.z));

			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 3 * sizeof(float), sizeof(float), &(splat.colour.x));
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 4 * sizeof(float), sizeof(float), &(splat.colour.y));
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 5 * sizeof(float), sizeof(float), &(splat.colour.z));
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 6 * sizeof(float), sizeof(float), &(splat.colour.w));
			splat.sent = true;
		}
	}

	for (std::tuple<int,Vector4>& splat : world->splatsToChangeColour) {
		int offset = std::get<0>(splat) * sizeof(float) * 7;
		Vector4 colour = std::get<1>(splat);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 3 * sizeof(float), sizeof(float), &(colour.x));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 4 * sizeof(float), sizeof(float), &(colour.y));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 5 * sizeof(float), sizeof(float), &(colour.z));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 6 * sizeof(float), sizeof(float), &(colour.w));
	}

	for (std::tuple<int, Vector4>& splat : world->cleanedSplats) {
		int offset = std::get<0>(splat) * sizeof(float) * 7;
		Vector4 colour = std::get<1>(splat);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 3 * sizeof(float), sizeof(float), &(colour.x));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 4 * sizeof(float), sizeof(float), &(colour.y));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 5 * sizeof(float), sizeof(float), &(colour.z));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset + 6 * sizeof(float), sizeof(float), &(colour.w));
	}


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	world->splatsToChangeColour.clear();
	world->cleanedSplats.clear();
}


