#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "Assets.h"
#include "Wall.h"
#include "SecurityGuard.h"


using namespace NCL;
using namespace CSC8508;

PaintingGame::PaintingGame(bool online) {
	world = new GameWorld();

	/* Code for changing physics system paramaters
	// Create the world settings 
	PhysicsWorld::WorldSettings settings; 
	settings.defaultVelocitySolverNbIterations = 20; 
	settings.isSleepingEnabled = false; 
	settings.gravity = Vector3(0,-9.81, 0);
	*/
	thirdPersonCamera = true;
	is_Networked = online;

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

	delete renderer;
	delete world;
}

void PaintingGame::UpdateGame(float dt) {
	world->GetMainCamera()->UpdateCamera(dt);

	if (renderer->GetGameState() == GameTechRenderer::GameState::SplitScreen)
	{
		world->GetSecondCamera()->UpdateCamera(dt);
	}
	if (thirdPersonCamera)
	{
		for (int i = 0; i < 2; i++)
		{
			playerControllers[i]->Update(dt);
		}
	
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
		thirdPersonCamera = !thirdPersonCamera;
		InitCamera();
	}
	renderer->Render();
	world->UpdateWorld(dt);
	renderer->Update(dt);
	physicsWorld->update(dt);
	Debug::UpdateRenderables(dt);

}

void PaintingGame::InitCamera()
{
	float aspect_divide = (renderer->GetGameState() == GameTechRenderer::GameState::SplitScreen) ? 2.0f : 1.0f;

	if (thirdPersonCamera) {
		world->GetMainCamera()->SetThirdPersonCamera(players[0]);
	}
	else {
		world->GetMainCamera()->SetFirstPersonCamera();
	}

	world->GetMainCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	world->GetMainCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() / aspect_divide);

	if (thirdPersonCamera) {
		world->GetSecondCamera()->SetThirdPersonCamera(players[1]);
	}
	else {
		world->GetSecondCamera()->SetFirstPersonCamera();
	}

	world->GetSecondCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	world->GetSecondCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() / aspect_divide);
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

	world->AddGameObject(new Floor(physicsCommon, physicsWorld, Vector3(0, 0, 0), meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), 200));

	//for (int x = 0; x < 15; ++x) {
	//	world->AddGameObject(new Box(physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 2));
	//}
	AddStructureFromFile(Vector3(-75.0, 5.0f, -75.0f), "GameMaze3.txt");
	AddSecurityAI();

}


PlayerBase* PaintingGame::InitiliazePlayer() {
	players[0] = new PlayerBase(physicsCommon, physicsWorld, Vector3(10, 10, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 5);
	world->AddGameObject(players[0]);
	playerControllers[0] = new PlayerController(world->GetMainCamera(), players[0]);

	players[1] = new PlayerBase(physicsCommon, physicsWorld, Vector3(95, 10, 0), meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), 5);
	world->AddGameObject(players[1]);
	playerControllers[1] = new PlayerController(world->GetSecondCamera(), players[1]);

	return players[0];
}

PlayerBase* PaintingGame::InitialiseNetworkPlayer() {
	
	netPlayer = new PlayerBase(physicsCommon, physicsWorld, Vector3(0, 50, 10), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 5);
	world->AddGameObject(netPlayer);
	return netPlayer;
}

GameTechRenderer* PaintingGame::GetGameTechRenderer()
{
	return renderer;
}

void PaintingGame::AddStructureFromFile(const NCL::Maths::Vector3& position, const std::string filename)
{
	std::ifstream infile(Assets::DATADIR + filename);
	int nodeSize, gridWidth, gridHeight;

	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	std::vector<char> grid;
	std::vector<char> grid_backup = grid;

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			char type = '.';
			infile >> type;
			grid.push_back(type);
		}
	}

	infile.close();

	// Trying to fuse horizontal walls into one to save on creating too many GameObjects
	float wall_counter = 1.0f;
	int column_counter = 1.0f;
	Vector3 old_pos;

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			int index = y * gridWidth + x;
			char type = grid[index];

			Vector3 pos = Vector3((float)(x * nodeSize), 0, (float)(y * nodeSize));

			if (wall_counter > 1)
			{
				pos = old_pos;
				pos.x += (nodeSize * (wall_counter - 1)) / 2.0f;
			}
			else
			{
				old_pos = pos;
			}

			if (type == '.')
			{
				wall_counter = 1.0f;
				continue;
			}

			if ((x < gridWidth - 1) && (grid[index + 1] == type)) // TODO: Bounds check
			{
				wall_counter++;
				continue;
			}
			else if ((wall_counter == 1) && (y < gridHeight - 1) && (grid[index + gridWidth] == type))
			{
				column_counter = 0;
				for (int i = index; i < grid.size(); i += gridWidth)
				{
					if (grid[i] == type)
					{
						column_counter++;
						grid[i] = '.';
						continue;
					}

					break;
				}

				pos.z += (nodeSize * (column_counter - 1)) / 2.0f;
			}

			if (type == 'x')
			{
				// Wall
				world->AddGameObject(new Wall(physicsCommon, physicsWorld, position + pos, meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), Vector3((nodeSize * wall_counter) , 10.0f, (nodeSize * column_counter) )));

			}

			column_counter = 1;
			wall_counter = 1; // reset after building walls
		}
	}
	return;
}

PlayerBase* PaintingGame::GetPlayer()
{
	return players[0];
}

void PaintingGame::AddSecurityAI()
{
	world->AddGameObject(new SecurityGuard(physicsCommon, physicsWorld, "Security Guard", Vector3(-70.0f, 5.0f, 60.0f), meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), Vector3(2, 2, 2), GetPlayer()));
}

