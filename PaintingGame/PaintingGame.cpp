#include "PaintingGame.h"
#include <Terrain.h>

using namespace NCL;
using namespace CSC8503;

PaintingGame::PaintingGame() {
	world = new GameWorld();
#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
#else 
	renderer = new GameTechRenderer(*world);
#endif
	physics = new PhysicsSystem(*world);

	forceMagnitude = 10.0f;
	inSelectionMode = false;

	InitialiseAssets();
	physics->UseGravity(useGravity);
	renderer->UseFog(useFog);
	hedgeMaze = nullptr;
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
	delete hedgeMaze;
	delete physics;
	delete renderer;
	delete world;
}

void PaintingGame::UpdateGame(float dt) {

	world->GetMainCamera()->UpdateCamera(dt);

	renderer->Render();
	world->UpdateWorld(dt);
	renderer->Update(dt);

	physics->Update(dt);

	remainingTime = remainingTime - dt;
	Debug::UpdateRenderables(dt);
}

void PaintingGame::InitCamera() {
	world->GetMainCamera()->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
	world->GetMainCamera()->SetFirstPersonCamera(); 
	world->GetMainCamera()->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
	lockedObject = nullptr;
}

void PaintingGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	if (!hedgeMaze) delete hedgeMaze;

	//InitMixedGridWorld(1, 1, 3.5f, 3.5f);
	remainingTime = 2 * 60;

	TerrainTexturePack terrainTexturePack(textures.at("terrainSplatMap"), textures.at("terrainRTex"), textures.at("terrainGTex"), textures.at("terrainBTex"), textures.at("terrainBgTex"));
	world->AddGameObject(new Terrain(Vector2(), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));
	world->AddGameObject(new Terrain(Vector2(0, 1), meshes.at("terrainMesh"), terrainTexturePack, shaders.at("terrainShader")));

	//InitGameExamples();
	//InitDefaultFloor();
	//testStateObject = AddStateObjectToWorld(Vector3(15, 10, 0));
	//AddNavigationGrid();
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* PaintingGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject("Floor");

	Vector3 floorSize = Vector3(200, 2, 200);
	AABBVolume* volume = new AABBVolume(floorSize);
	//OBBVolume* volume = new OBBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), meshes.at("cubeMesh"), shaders.at("basicShader")));
	int numSubMeshes = meshes.at("cubeMesh")->GetSubMeshCount();
	for (int subMeshIndex = 0; subMeshIndex < numSubMeshes; ++subMeshIndex) {
		floor->GetRenderObject()->AddTexture(subMeshIndex, textures.at("floorTex"), "mainTex");
	}
	//floor->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->SetLayer(Layer::StaticObjects);

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* PaintingGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("Sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), meshes.at("sphereMesh"), shaders.at("basicShader")));
	sphere->GetRenderObject()->AddTexture(0, textures.at("basicTex"), "mainTex");
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();
	//sphere->SetLayer(Layer::Spheres);

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* PaintingGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject("Cube");

	AABBVolume* volume = new AABBVolume(dimensions);
	//OBBVolume* volume = new OBBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), meshes.at("cubeMesh"), shaders.at("basicShader")));
	cube->GetRenderObject()->AddTexture(0, textures.at("basicTex"), "mainTex");
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	if (rand() % 2) {
		cube->GetPhysicsObject()->SetElasticity(1.0f);
		cube->GetRenderObject()->SetColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else {
		cube->GetPhysicsObject()->SetElasticity(0.2f);
		cube->GetRenderObject()->SetColour(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	//cube->SetLayer(Layer::Cubes);
	world->AddGameObject(cube);

	return cube;
}

//void PaintingGame::AddCoinsToWorld(const Vector3& position) {
//	world->AddGameObject(new Coin(position, meshes.at("coinMesh"), textures.at("coinTex"), shaders.at("basicShader")));
//}


//StateGameObject* PaintingGame::AddStateObjectToWorld(const Vector3& position) {
//	float meshSize = 3.0f;
//	float inverseMass = 0.5f;
//
//	StateGameObject* character = new StateGameObject();
//
//	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
//	//CapsuleVolume* volume = new CapsuleVolume(0.9 * meshSize, 0.3 * meshSize);
//	character->SetBoundingVolume((CollisionVolume*)volume);
//
//	character->GetTransform()
//		.SetScale(Vector3(meshSize, meshSize, meshSize))
//		.SetPosition(position);
//
//	character->SetRenderObject(new RenderObject(&character->GetTransform(), meshes.at("enemyMesh"), nullptr, shaders.at("basicShader")));
//	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
//
//	character->GetPhysicsObject()->SetInverseMass(inverseMass);
//	character->GetPhysicsObject()->InitSphereInertia();
//	character->SetLayer(Layer::OtherObjects);
//
//	world->AddGameObject(character);
//
//	return character;
//}

void PaintingGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(140, -20, 130));
}

void PaintingGame::InitGameExamples() {
	//AddPlayerToWorld(Vector3(15, 5, 0));
	//AddCoinsToWorld(Vector3(5, -15, 0));
}

void PaintingGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0));
}

void PaintingGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}

void PaintingGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols + 1; ++x) {
		for (int z = 1; z < numRows + 1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around.

*/
bool PaintingGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true, NULL)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				Debug::DrawLine(ray.GetPosition(), closestCollision.collidedAt, Vector4(1, 0, 1, 1), 2000);
				return true;
			}
			else {
				return false;
			}
		}
		if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
			if (selectionObject) {
				if (lockedObject == selectionObject) {
					lockedObject = nullptr;
				}
				else {
					lockedObject = selectionObject;
				}
			}
		}
	}
	else {
		Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void PaintingGame::MoveSelectedObject() {
	Debug::Print("Click Force:" + std::to_string(forceMagnitude), Vector2(5, 90));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true, NULL)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}


