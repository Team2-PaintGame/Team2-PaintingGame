#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"

using namespace NCL;
using namespace CSC8508;

PaintingGame::PaintingGame() {
	world = new GameWorld();

	/* Code for changing physics system paramaters
	// Create the world settings 
	PhysicsWorld::WorldSettings settings; 
	settings.defaultVelocitySolverNbIterations = 20; 
	settings.isSleepingEnabled = false; 
	settings.gravity = Vector3(0,-9.81, 0);
	*/
	thirdPersonCamera = false;

	physicsWorld = physicsCommon.createPhysicsWorld(/*settings*/);

#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
#else 
	renderer = new GameTechRenderer(*world, physicsWorld);
#endif
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
	meshes.insert(std::make_pair("sphereMesh", renderer->LoadMesh("sphere.msh")));
	meshes.insert(std::make_pair("goatMesh", renderer->LoadMesh("goat.msh")));
	meshes.insert(std::make_pair("enemyMesh", renderer->LoadMesh("Keeper.msh")));
	meshes.insert(std::make_pair("coinMesh", renderer->LoadMesh("coin.msh")));
	meshes.insert(std::make_pair("capsuleMesh", renderer->LoadMesh("capsule.msh")));
	meshes.insert(std::make_pair("terrainMesh", renderer->LoadHeightMap("noise.png")));

	meshMaterials.insert(std::make_pair("goatMat", new MeshMaterial("goat.mat")));
	//meshMaterials.at("goatMat")->LoadTextures();

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

	//renderer->AddHudTextures("wolf_color.png", Vector2(0.5,0.5), Vector2(0.25,0.25));
	//renderer->AddHudTextures("wolf_color.png", Vector2(-0.5, 0.5), Vector2(0.25, 0.25));


	InitWorld();
	InitCamera();
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
	delete renderer;
	delete world;
}

void PaintingGame::UpdateGame(float dt) {

	world->GetMainCamera()->UpdateCamera(dt);

	if (thirdPersonCamera)
	{
		playerController->Update(dt);
	}

	renderer->Render();
	world->UpdateWorld(dt);
	renderer->Update(dt);
	physicsWorld->update(dt);

	remainingTime = remainingTime - dt;
	Debug::UpdateRenderables(dt);
}

void PaintingGame::InitCamera() {
	world->GetMainCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);

	if (thirdPersonCamera) {
		world->GetMainCamera()->SetThirdPersonCamera(player);
	}
	else {
		world->GetMainCamera()->SetFirstPersonCamera();
	}

	world->GetMainCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();

	remainingTime = 2 * 60;

	//TerrainTexturePack terrainTexturePack(textures.at("terrainSplatMap"), textures.at("terrainRTex"), textures.at("terrainGTex"), textures.at("terrainBTex"), textures.at("terrainBgTex"));
	//world->AddGameObject(new Terrain(*physicsCommon, physicsWorld, Vector2(), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));
	//world->AddGameObject(new Terrain(*physicsCommon, physicsWorld, Vector2(0, 1), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));

	InitiliazePlayer();

	world->AddGameObject(new Floor(physicsCommon, physicsWorld, Vector3(0, 0, 0), meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), 200));

	for (int x = 0; x < 15; ++x) {
		world->AddGameObject(new Box(physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 2));
	}
}

void PaintingGame::InitiliazePlayer() {
	player = new PlayerBase(physicsCommon, physicsWorld, Vector3(0, 50, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 5);
	world->AddGameObject(player);

	playerController = new PlayerController(world->GetMainCamera(), player);
}


