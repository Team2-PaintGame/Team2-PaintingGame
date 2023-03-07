#include "PaintingGame.h"
#include <Terrain.h>
#include <Debug.h>
#include "Box.h"
#include "Floor.h"
#include "Assets.h"
#include "Wall.h"
#include "../GameObjects/MuseumItem.h"
#include "SecurityGuard.h"
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

	menuHandler = menu;

	this->physicsCommon = physicsCommon;
	this->physicsWorld = this->physicsCommon->createPhysicsWorld();

	renderer = render;

	renderer->SetPhysicsWorld(physicsWorld);

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
//	meshes.insert(std::make_pair("floorMesh", renderer->LoadMesh("Arena.msh")));
	meshes.insert(std::make_pair("floorMesh", renderer->LoadMesh("BasicLVL.msh")));  

	meshes.insert(std::make_pair("cubeMesh", renderer->LoadMesh("cube.msh")));
	meshes.insert(std::make_pair("mainChar", renderer->LoadMesh("Aj_TPose.msh")));
	meshes.insert(std::make_pair("sphereMesh", renderer->LoadMesh("sphere.msh")));
	meshes.insert(std::make_pair("goatMesh", renderer->LoadMesh("goat.msh")));
	meshes.insert(std::make_pair("enemyMesh", renderer->LoadMesh("Keeper.msh")));
	meshes.insert(std::make_pair("coinMesh", renderer->LoadMesh("coin.msh")));
	meshes.insert(std::make_pair("capsuleMesh", renderer->LoadMesh("capsule.msh")));
	meshes.insert(std::make_pair("terrainMesh", renderer->LoadHeightMap("noise.png")));

	
//	meshes.insert(std::make_pair("throneMesh", renderer->LoadMesh("SanctumThrone.msh")));
//	textures.insert(std::make_pair("throneColour", renderer->LoadTexture("SanctumThrone/InSanct_Max_Throne_A_Colour.tga")));
//	textures.insert(std::make_pair("throneMetal", renderer->LoadTexture("SanctumThrone/InSanct_Max_Throne_A_Metal.tga")));
//	textures.insert(std::make_pair("throneNormal", renderer->LoadTexture("SanctumThrone/InSanct_Max_Throne_A_Normal.tga")));

//	meshes.insert(std::make_pair("catMesh", renderer->LoadMesh("SanctumCat.msh")));
//	textures.insert(std::make_pair("catColour", renderer->LoadTexture("SanctumThrone/InSanct_Max_Cat_Colour.tga")));

	textures.insert(std::make_pair("basicTex", renderer->LoadTexture("checkerboard.png")));

	meshMaterials.insert(std::make_pair("goatMat", new MeshMaterial("goat.mat")));
	meshMaterials.insert(std::make_pair("mainCharMat", new MeshMaterial("Aj_TPose.mat")));
	//meshMaterials.at("goatMat")->LoadTextures();
	//meshMaterials.at("mainCharMat")->LoadTextures();

	//meshAnimations.insert(std::make_pair("mainCharTauntAnim", new MeshAnimation("Taunt.anm")));
	meshAnimations.insert(std::make_pair("mainCharIdleAnim", new MeshAnimation("AJIdle.anm")));
	meshAnimations.insert(std::make_pair("mainCharRunAnim", new MeshAnimation("AJRun.anm")));


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
	textures.insert(std::make_pair("boxTGA", renderer->LoadTexture("Maximilian_BodyHands_Albedo.jpg")));


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

	if(menuHandler->GetGameState() != GameState::ExitGame)
	physicsCommon->destroyPhysicsWorld(physicsWorld);
	world->ClearAndErase();
}

void PaintingGame::UpdateGame(float dt) {
	world->UpdateWorld(dt);
	menuHandler->Update(dt);
	renderer->Render();
	renderer->Update(dt);
	physicsWorld->update(dt);
	Debug::UpdateRenderables(dt);
}

void PaintingGame::InitCamera(Camera& camera, PlayerBase& focus, float aspect_multiplier)
{
	camera.SetThirdPersonCamera(&focus);

	camera.SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	camera.SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect() * aspect_multiplier);
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();

	world->AddGameObject(new Floor(*physicsCommon, physicsWorld, Vector3(0, 0, 0), meshes.at("floorMesh"), CreateConcaveCollision("floorMesh"),  textures.at("basicTex"), shaders.at("basicShader"), 1));

	for (int x = 0; x < 15; ++x) {
		world->AddGameObject(new Box(*physicsCommon, physicsWorld, Vector3(0, 10, 0), meshes.at("cubeMesh"), textures.at("boxTGA"), shaders.at("basicShader"), 2));
	}


	//world->AddGameObject(new Floor(*physicsCommon, physicsWorld, Vector3(0, 0, 0), meshes.at("sceneMesh"), textures.at("basicTex"), shaders.at("basicShader"), 1));

	
	//AddStructureFromFile(Vector3(-75.0, 5.0f, -75.0f), "SplatAtTheMuseum.txt");
	//AddSecurityAI();
	//world->AddGameObject(new MuseumItem(*physicsCommon, physicsWorld, Vector3(25, 25, 25), meshes.at("throneMesh"), textures.at("basicTex"), shaders.at("basicShader"), Vector3(10, 10, 10), "Throne"));
	//world->AddGameObject(new MuseumItem(*physicsCommon, physicsWorld, Vector3(50, 25, 50), meshes.at("catMesh"), textures.at("basicTex"), shaders.at("basicShader"), Vector3(10, 10, 10), "Cat"));
}

PlayerBase* PaintingGame::CreatePlayer(NCL::CSC8508::Vector3 position) {
	AnimationController* animController = new AnimationController();
	animController->SetIdleAnimation(meshAnimations.at("mainCharIdleAnim"));
	animController->SetRunAnimation(meshAnimations.at("mainCharRunAnim"));
	//animController->SetTauntAnimation(meshAnimations.at("mainCharTauntAnim"));

	//SetColorOfMesh(meshes.at("mainChar"), Debug::RED);
	return new PlayerBase(*physicsCommon, physicsWorld, position, meshes.at("mainChar"), meshMaterials.at("mainCharMat"), animController, shaders.at("skinningShader"), 5);
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
	NCL::CSC8508::Vector3 old_pos;

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			int index = y * gridWidth + x;
			char type = grid[index];

			NCL::CSC8508::Vector3 pos = NCL::CSC8508::Vector3((float)(x * nodeSize), 0, (float)(y * nodeSize));

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
				world->AddGameObject(new Wall(*physicsCommon, physicsWorld, position + pos, meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), Vector3((nodeSize * wall_counter) , 10.0f, (nodeSize * column_counter) )));

			}

			column_counter = 1;
			wall_counter = 1; // reset after building walls
		}
	}
	return;
}


void PaintingGame::AddSecurityAI(NCL::CSC8508::Vector3 position, PlayerBase* target1, PlayerBase* target2) // Vector3(-70.0f, 5.0f, 60.0f) // Change the hardcoded two targets
{
	if(menuHandler->GetGameState() != GameState::MainMenu)
	world->AddGameObject(new SecurityGuard(*physicsCommon, physicsWorld, "Security Guard", position, meshes.at("cubeMesh"), textures.at("basicTex"), shaders.at("basicShader"), Vector3(2, 2, 2), target1, target2));
}

void NCL::CSC8508::PaintingGame::SetColorOfMesh(MeshGeometry* mesh, Vector4 color)
{
	vector<Vector4> vertexColor;
	for (int i = 0; i < mesh->GetVertexCount(); i++)
	{
		vertexColor.emplace_back(color);
	}

	const SubMesh* subMesh = mesh->GetSubMesh(0);
	vector<unsigned int> indices = mesh->GetIndexData();

	int start = subMesh->start;
	int end = start + subMesh->count;
	int A, B, C = 0;
	for (int i = start; i < end; i += 3)
	{
		A = indices[i + 0];
		B = indices[i + 1];
		C = indices[i + 2];
		vertexColor[A] = Vector4(0, 1, 0, 1);
		vertexColor[B] = Vector4(0, 1, 0, 1);
		vertexColor[C] = Vector4(0, 1, 0, 1);
	}


	mesh->SetVertexColours(vertexColor);
	mesh->UploadToGPU();
} 