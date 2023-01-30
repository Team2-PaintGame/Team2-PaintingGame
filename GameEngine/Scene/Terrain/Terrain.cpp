#include "Terrain.h"
#include "RenderObject.h"
#include "PhysicsObject.h"
#include "SphereVolume.h"

using namespace NCL;
using namespace Rendering;

Terrain::Terrain(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector2 gridPosition, MeshGeometry* mesh, TerrainTexturePack texturePack, ShaderBase* shader, int size): GameObject(physicsCommon, physicsWorld) {
	transform.SetPosition(Vector3(gridPosition.x * size, 0, gridPosition.y * size));
	transform.SetPosition(Matrix4::Translation(Vector3(-size / 2, 0, -size / 2)) * transform.GetPosition()); //for centering the terrain around this point

	transform.SetScale(Vector3(size, 1, size));
	name = "terrain";
	renderObject = new RenderObject(&transform, mesh, shader);
	
	renderObject->AddTexture(0, texturePack.rTex, "rTex");
	renderObject->AddTexture(0, texturePack.gTex, "gTex");
	renderObject->AddTexture(0, texturePack.bTex, "bTex");
	renderObject->AddTexture(0, texturePack.bgTex, "bgTex");
	renderObject->AddTexture(0, texturePack.splatMap, "splatMapTex");

	/*float inverseMass = 0.0f;
	physicsObject = new PhysicsObject(&transform, NULL);
	physicsObject->SetInverseMass(inverseMass);
	physicsObject->InitSphereInertia();	*/
}
