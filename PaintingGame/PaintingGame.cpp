#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "InputController.h"
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
	gamePad = new Gamepad();
	animController = new AnimationController();

	/* Code for changing physics system paramaters
	// Create the world settings 
	PhysicsWorld::WorldSettings settings; 
	settings.defaultVelocitySolverNbIterations = 20; 
	settings.isSleepingEnabled = false; 
	settings.gravity = Vector3(0,-9.81, 0);
	*/
	thirdPersonCamera = true;
	is_Networked = online;
	menuHandler = menu;

	this->physicsCommon = physicsCommon;
	this->physicsWorld = this->physicsCommon->createPhysicsWorld(/*settings*/);

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
	meshes.insert(std::make_pair("floorMesh", renderer->LoadMesh("Arena.msh")));
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

	//renderer->AddHudTextures("wolf_color.png", Vector2(0.5,0.5), Vector2(0.25,0.25));
	//renderer->AddHudTextures("wolf_color.png", Vector2(-0.5, 0.5), Vector2(0.25, 0.25));

	InitWorld();
	if (!is_Networked) // networked game handles cameras itself
	{
		InitCamera();
	}
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
	for (auto& pc : playerControllers)
	{
		if (pc)	delete pc;
	}

	//delete renderer;
	//delete world;
	//delete physicsCommon->destroyConcaveMeshShape(arenaConcaveMeshCollision);
}

void PaintingGame::UpdateGame(float dt) {
	world->GetMainCamera()->UpdateCamera(dt);

	if (menuHandler->GetGameState() != GameState::MainMenu) // ugly temporary if by an ugly temporary programmer
	{
		if (thirdPersonCamera)
		{
			for (int i = 0; i < numberOfPlayerControllers; i++)
			{
				playerControllers[i]->Update(dt);
			}

		}

		if (menuHandler->GetGameState() == GameState::SplitScreen) {
			numberOfPlayerControllers = 2;
			world->GetSecondCamera()->UpdateCamera(dt);
		}
		else if (menuHandler->GetGameState() == GameState::SinglePlayer) {
			numberOfPlayerControllers = 1;
		}


		physicsWorld->update(dt);
		world->UpdateWorld(dt);

	}

	menuHandler->Update(dt);
	renderer->Render();
	renderer->Update(dt);
	Debug::UpdateRenderables(dt);
}

void PaintingGame::InitCamera()
{
	//float aspect_divide = renderer->GetGameState() == GameTechRenderer::GameState::SplitScreen ? 2.0f : 1.0f;

	if (thirdPersonCamera) {
		world->GetMainCamera()->SetThirdPersonCamera(players[0]);
	}
	else {
		world->GetMainCamera()->SetFirstPersonCamera();
	}

	world->GetMainCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	world->GetMainCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());

	
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();

	//TerrainTexturePack terrainTexturePack(textures.at("terrainSplatMap"), textures.at("terrainRTex"), textures.at("terrainGTex"), textures.at("terrainBTex"), textures.at("terrainBgTex"));
	//world->AddGameObject(new Terrain(*physicsCommon, physicsWorld, Vector2(), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));
	//world->AddGameObject(new Terrain(*physicsCommon, physicsWorld, Vector2(0, 1), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));

	if (!is_Networked) // Networked Game handles its own player init
	{
		InitiliazePlayer();
	}

	world->AddGameObject(new Floor(*physicsCommon, physicsWorld, Vector3(-30, 0, 20), meshes.at("floorMesh"),CreateConcaveCollision("floorMesh"), textures.at("basicTex"), shaders.at("basicShader"), 1));

	for (int x = 0; x < 15; ++x) {
		world->AddGameObject(new Box(*physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 2));
	}
}


PlayerBase* PaintingGame::InitiliazePlayer() {
	animController->SetIdleAnimation(meshAnimations.at("mainCharIdleAnim"));
	animController->SetRunAnimation(meshAnimations.at("mainCharRunAnim"));
	animController->SetTauntAnimation(meshAnimations.at("mainCharTauntAnim"));
	players[0] = new PlayerBase(*physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("mainChar"), textures.at("basicTex"), animController, shaders.at("skinningShader"), 5);
	

	world->AddGameObject(players[0]);
	playerControllers[0] = new PlayerController(world->GetMainCamera(), players[0],NULL);

	return players[0];
}

PlayerBase* PaintingGame::InitialiseNetworkPlayer() {
	animController->SetIdleAnimation(meshAnimations.at("mainCharIdleAnim"));
	animController->SetRunAnimation(meshAnimations.at("mainCharRunAnim"));
	animController->SetTauntAnimation(meshAnimations.at("mainCharTauntAnim"));
	netPlayer = new PlayerBase(*physicsCommon, physicsWorld, Vector3(0, 50, 10), meshes.at("mainChar"), textures.at("basicTex"), animController, shaders.at("skinningShader"), 5);
	world->AddGameObject(netPlayer);
	return netPlayer;
}

reactphysics3d::ConcaveMeshShape* NCL::CSC8508::PaintingGame::CreateConcaveCollision(std::string meshName)
{
	float nbvertices = meshes.at(meshName)->GetVertexCount();
	int indices = meshes.at(meshName)->GetIndexCount();
	int trianglesCount = indices / 3;

	const void* meshVertStart = meshes.at(meshName)->GetPositionData().data();
	const void* meshIndexStart = meshes.at(meshName)->GetIndexData().data();

	reactphysics3d::TriangleVertexArray* triangleArray =
		new reactphysics3d::TriangleVertexArray(nbvertices, meshVertStart, sizeof(Vector3), trianglesCount,
			meshIndexStart, 3 * sizeof(int),
			reactphysics3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
			reactphysics3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

	reactphysics3d::TriangleMesh* triangleMesh = physicsCommon->createTriangleMesh();

	// Add the triangle vertex array to the triangle mesh 
	triangleMesh->addSubpart(triangleArray);

	// Create the concave mesh shape 
	arenaConcaveMeshCollision = physicsCommon->createConcaveMeshShape(triangleMesh);
	return arenaConcaveMeshCollision;
}

GameTechRenderer* PaintingGame::GetGameTechRenderer()
{
	return renderer;
}

PlayerBase* PaintingGame::InitSecondPlayer() {
	animController->SetIdleAnimation(meshAnimations.at("mainCharIdleAnim"));
	animController->SetRunAnimation(meshAnimations.at("mainCharRunAnim"));
	animController->SetTauntAnimation(meshAnimations.at("mainCharTauntAnim"));
	players[1] = new PlayerBase(*physicsCommon, physicsWorld, Vector3(10, 10, 0), meshes.at("mainChar"), textures.at("basicTex"), animController, shaders.at("skinningShader"), 5);
	world->AddGameObject(players[1]);
	playerControllers[1] = new PlayerController(world->GetSecondCamera(), players[1], gamePad);

	return players[1];
}

void PaintingGame::InitSecondCamera() {

	if (thirdPersonCamera) {
		world->GetSecondCamera()->SetThirdPersonCamera(players[1]);
		world->GetSecondCamera()->SetGamePad(gamePad);
	}
	else {
		world->GetSecondCamera()->SetFirstPersonCamera();
	}

	world->GetSecondCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	world->GetSecondCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() / 2.0f);
}

void PaintingGame::DestroySecondPlayer() {

	players[1]->~PlayerBase();
	world->GetSecondCamera()->~Camera();
}


