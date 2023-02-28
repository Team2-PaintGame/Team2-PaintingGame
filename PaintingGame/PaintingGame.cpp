#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "AnimationController.h"
#include "MenuHandler.h"

using namespace NCL;
using namespace CSC8508;

#ifdef USEVULKAN
PaintingGame::PaintingGame(GameTechVulkanRenderer* render, GameWorld* world, reactphysics3d::PhysicsWorld* physicsWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu, bool online = false)
#else
PaintingGame::PaintingGame(GameTechRenderer* render, GameWorld* game_world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu, bool online)
#endif
{
	world = game_world;

	menuHandler = menu;

	this->physicsCommon = physicsCommon;
	this->physicsWorld = this->physicsCommon->createPhysicsWorld();

	renderer = render;

	forceMagnitude = 10.0f;

	InitialiseAssets();
	physicsWorld->setIsGravityEnabled(useGravity);
	renderer->UseFog(useFog);

	renderer->settings.SetIsDebugRenderingModeEnabled(true);
	renderer->settings.debugRendererSettings.SetIsCollisionShapeDisplayed(true);
	renderer->settings.debugRendererSettings.SetIsBroadPhaseAABBDisplayed(true);
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes,
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void PaintingGame::InitialiseAssets() {
	meshes.insert(std::make_pair("cubeMesh", renderer->LoadMesh("cube.msh")));
	meshes.insert(std::make_pair("mainChar", renderer->LoadMesh("Male_Guard.msh")));
	meshes.insert(std::make_pair("sphereMesh", renderer->LoadMesh("sphere.msh")));
	meshes.insert(std::make_pair("goatMesh", renderer->LoadMesh("goat.msh")));
	meshes.insert(std::make_pair("enemyMesh", renderer->LoadMesh("Keeper.msh")));
	meshes.insert(std::make_pair("coinMesh", renderer->LoadMesh("coin.msh")));
	meshes.insert(std::make_pair("capsuleMesh", renderer->LoadMesh("capsule.msh")));
	meshes.insert(std::make_pair("terrainMesh", renderer->LoadHeightMap("noise.png")));

	meshMaterials.insert(std::make_pair("goatMat", new MeshMaterial("goat.mat")));
	meshMaterials.insert(std::make_pair("mainCharMat", new MeshMaterial("goat.mat")));
	//meshMaterials.at("goatMat")->LoadTextures();
	//meshMaterials.at("mainCharMat")->LoadTextures();

	meshAnimations.insert(std::make_pair("mainCharTauntAnim", new MeshAnimation("Taunt.anm")));
	meshAnimations.insert(std::make_pair("mainCharIdleAnim", new MeshAnimation("Idle1.anm")));
	meshAnimations.insert(std::make_pair("mainCharRunAnim", new MeshAnimation("StepForward.anm")));


	textures.insert(std::make_pair("basicTex", renderer->LoadTexture("checkerboard.png")));
	textures.insert(std::make_pair("grassTex", renderer->LoadTexture("grass.jpg")));
	textures.insert(std::make_pair("coinTex", renderer->LoadTexture("coin.png")));
	textures.insert(std::make_pair("floorTex", renderer->LoadTexture("ground.png")));
	textures.insert(std::make_pair("goatTex", renderer->LoadTexture("goat1.jpg")));
	textures.insert(std::make_pair("doorTex", renderer->LoadTexture("door.jpg")));

	textures.insert(std::make_pair("terrainSplatMap", renderer->LoadTexture("Terrain/splatmap.png")));
	textures.insert(std::make_pair("terrainRTex", renderer->LoadTexture("Terrain/rTex_mud.jpg")));
	textures.insert(std::make_pair("terrainGTex", renderer->LoadTexture("Terrain/gTex_mudGrass.jpg")));
	textures.insert(std::make_pair("terrainBTex", renderer->LoadTexture("Terrain/bTex_path.jpg")));
	textures.insert(std::make_pair("terrainBgTex", renderer->LoadTexture("Terrain/bgTex_grass.jpg")));


	shaders.insert(std::make_pair("basicShader", renderer->LoadShader("scene.vert", "scene.frag")));
	shaders.insert(std::make_pair("terrainShader", renderer->LoadShader("terrain.vert", "terrain.frag")));
	shaders.insert(std::make_pair("skinningShader", renderer->LoadShader("skinning.vert", "scene.frag")));
}

PaintingGame::~PaintingGame() {
	for (auto const& [key, val] : meshes) {
		delete val;
	}

	for (auto const& [key, val] : textures) {
		delete val;
	}

	for (auto const& [key, val] : shaders) {
		delete val;
	}

	for (const auto& [key, val] : meshMaterials) {
		delete val;
	}
	for (const auto& [key, val] : meshAnimations) {
		delete val;
	}

	
	world->ClearAndErase();
	physicsCommon->destroyPhysicsWorld(physicsWorld);
}

void PaintingGame::InitCamera(Camera& camera, PlayerBase& focus, float aspect_multiplier)
{
	camera.SetThirdPersonCamera(&focus);

	camera.SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	camera.SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * aspect_multiplier);
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();

	world->AddGameObject(new Floor(*physicsCommon, physicsWorld, Vector3(0, 0, 0), meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), 200));

	for (int x = 0; x < 15; ++x) {
		world->AddGameObject(new Box(*physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 2));
	}
}

void PaintingGame::UpdateGame(float dt) {
	world->UpdateWorld(dt);
	menuHandler->Update(dt);
	renderer->Render();
	renderer->Update(dt);
	physicsWorld->update(dt);
	Debug::UpdateRenderables(dt);
}

PlayerBase* PaintingGame::CreatePlayer(Vector3 position) {
	AnimationController* animController = new AnimationController();
	animController->SetIdleAnimation(meshAnimations.at("mainCharIdleAnim"));
	animController->SetRunAnimation(meshAnimations.at("mainCharRunAnim"));
	animController->SetTauntAnimation(meshAnimations.at("mainCharTauntAnim"));

	return new PlayerBase(*physicsCommon, physicsWorld, position, meshes.at("mainChar"), textures.at("basicTex"), animController, shaders.at("skinningShader"), 5);
}

GameTechRenderer* PaintingGame::GetGameTechRenderer()
{
	return renderer;
}
