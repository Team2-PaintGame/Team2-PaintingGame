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
	
	Player* player; 
	if (team == Team::Red) {
		player =  new Player(physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetMeshMaterial("redMainCharMat"), assets->GetShader("skinningShader"), animations, 5, CreateGun(position, team));
	}

	else { //blue
		player =  new Player(physicsCommon, physicsWorld, position, assets->GetMesh("mainChar"), assets->GetMeshMaterial("blueMainCharMat"), assets->GetShader("skinningShader"), animations, 5, CreateGun(position, team));
	}

	world->AddGameObject(player);
	return player;
}

FocusPoint* PaintingGame::CreateFocusPoint() {
	return new FocusPoint(physicsCommon, physicsWorld, assets->GetMesh("quadMesh"), assets->GetTexture("gunFocusTex"), assets->GetShader("hudShader"), Vector2(0.05));
}

Gun* PaintingGame::CreateGun(Vector3 position, Team team) {
	Gun* gun;
	Ink* ink = CreateInkStream(physicsCommon, physicsWorld, Vector3(0, 10, 0), assets->GetMesh("sphereMesh"), Vector4(0, 1, 0, 1), assets->GetShader("inkShader"));

	if (team == Team::Red) {
		gun = new Gun(physicsCommon, physicsWorld, position, assets->GetMesh("gunMesh"), assets->GetMeshMaterial("gunMat"), assets->GetShader("basicShader"), 3, Debug::RED, ink);
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
	else { //blue
		gun = new Gun(physicsCommon, physicsWorld, position, assets->GetMesh("gunMesh"), assets->GetMeshMaterial("gunMat"), assets->GetShader("basicShader"), 3, Debug::BLUE, ink);
	}

	world->AddGameObject(gun);
	world->AddGameObject(ink);
	return gun;
}

	mesh->SetVertexColours(vertexColor);
	mesh->UploadToGPU();
} 