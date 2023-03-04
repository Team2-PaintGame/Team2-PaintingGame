#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "AnimationController.h"

using namespace NCL;
using namespace CSC8508;

PaintingGame::PaintingGame(GameAssets* assets) {
	this->assets = assets;
	this->physicsCommon = physicsCommon;
	this->physicsWorld = this->physicsCommon->createPhysicsWorld();
	physicsWorld->setIsGravityEnabled(useGravity);

	//renderer->settings.SetIsDebugRenderingModeEnabled(true);
	//renderer->settings.debugRendererSettings.SetIsCollisionShapeDisplayed(true);
	//renderer->settings.debugRendererSettings.SetIsBroadPhaseAABBDisplayed(true);
}

PaintingGame::~PaintingGame() {
	physicsCommon->destroyPhysicsWorld(physicsWorld);
	world->ClearAndErase();
}

//void PaintingGame::InitCamera(Camera& camera, PlayerBase& focus, float aspect_multiplier)
//{
//	camera.SetThirdPersonCamera(&focus);
//
//	camera.SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
//	camera.SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * aspect_multiplier);
//}

void PaintingGame::InitWorld() {
	world->ClearAndErase();

	world->AddGameObject(new Floor(*physicsCommon, physicsWorld, Vector3(0, 0, 0), assets->GetMesh("cubeMesh"), assets->GetTexture("basicTex"), assets->GetShader("basicShader"), 200));

	for (int x = 0; x < 15; ++x) {
		world->AddGameObject(new Box(*physicsCommon, physicsWorld, Vector3(0, 10, 0), assets->GetMesh("cubeMesh"), assets->GetTexture("doorTex"), assets->GetShader("basicShader"), 2));
	}
}

void PaintingGame::Update(float dt) {

	world->UpdateWorld(dt);
	physicsWorld->update(dt);
	Debug::UpdateRenderables(dt);
}

PlayerBase* PaintingGame::CreatePlayer(Vector3 position) {
	AnimationController* animController = new AnimationController();
	animController->SetIdleAnimation(assets->GetMeshAnimation("mainCharIdleAnim"));
	animController->SetRunAnimation(assets->GetMeshAnimation("mainCharRunAnim"));
	animController->SetTauntAnimation(assets->GetMeshAnimation("mainCharTauntAnim"));

	return new PlayerBase(*physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetTexture("basicTex"), animController, assets->GetShader("skinningShader"), 5);
}
