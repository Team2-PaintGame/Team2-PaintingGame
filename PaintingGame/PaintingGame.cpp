#include "PaintingGame.h"
#include <Terrain.h>

using namespace NCL;
using namespace CSC8508;

PaintingGame::PaintingGame() {
	world = new GameWorld();
#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
#else 
	renderer = new GameTechRenderer(*world);
#endif
	physics = new PhysicsSystem(*world);

	forceMagnitude = 10.0f;

	physicsCommon = new reactphysics3d::PhysicsCommon();
	p_world = physicsCommon->createPhysicsWorld();

	InitialiseAssets();
	physics->UseGravity(useGravity);
	renderer->UseFog(useFog);
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
	delete physics;
	delete renderer;
	delete world;

	delete physicsCommon;
}

void PaintingGame::UpdateGame(float dt) 
{

	//world->GetMainCamera()->UpdateCamera(dt);

	player_controller->Update(dt);

	renderer->Render();
	world->UpdateWorld(dt);
	renderer->Update(dt);

	physics->Update(dt);

	remainingTime = remainingTime - dt;
	Debug::UpdateRenderables(dt);
}

void PaintingGame::InitCamera() {
	world->GetMainCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	world->GetMainCamera()->SetThirdPersonCamera(player);
	world->GetMainCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();

	remainingTime = 2 * 60;

	TerrainTexturePack terrainTexturePack(textures.at("terrainSplatMap"), textures.at("terrainRTex"), textures.at("terrainGTex"), textures.at("terrainBTex"), textures.at("terrainBgTex"));
	world->AddGameObject(new Terrain(Vector2(), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));
	world->AddGameObject(new Terrain(Vector2(0, 1), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));

	InitiliazePlayer();

	//Create a rigid body in the world 
	reactphysics3d::Vector3 position(0, 20, 0);
	reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
	reactphysics3d::Transform transform(position, orientation);
	reactphysics3d::RigidBody* body = p_world->createRigidBody(transform); 
	const reactphysics3d::decimal timeStep = 1.0f / 60.0f;
	//Step the simulation a few steps 
	for (int i = 0; i < 20; i++) {
		p_world->update(timeStep); 
		//Get the updated position of the body 
		const reactphysics3d::Transform& transform = body->getTransform(); 
		const reactphysics3d::Vector3& position = transform.getPosition(); 
		//Display the position of the body 
		std::cout << "BodyPosition:( " << position.x << ", " << position.y << "," << position.z << ")" <<std::endl; 
	}
}

void PaintingGame::InitiliazePlayer() {
	player = new PlayerBase(Vector3(0, 10, 0), meshes.at("cubeMesh"), textures.at("doorTex"), shaders.at("basicShader"), 3);
	world->AddGameObject(player);

	player_controller = new PlayerController(world->GetMainCamera(), player);
}


