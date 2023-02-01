#include "Terrain.h"
#include "RenderObject.h"
#include "Utils.h"

using namespace NCL;
using namespace Rendering;

Terrain::Terrain(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector2 gridPosition, MeshGeometry* mesh, TerrainTexturePack texturePack, ShaderBase* shader, int size): GameObject(physicsCommon, physicsWorld, "Terrain") {
	transform.SetPosition(Vector3(gridPosition.x * size, 0, gridPosition.y * size));
	transform.SetPosition(Matrix4::Translation(Vector3(-size / 2, 0, -size / 2)) * transform.GetPosition()); //for centering the terrain around this point
	transform.SetScale(Vector3(size, 1, size));
	
	renderObject = new RenderObject(&transform, mesh, shader);
	renderObject->AddTexture(texturePack.rTex, "rTex");
	renderObject->AddTexture(texturePack.gTex, "gTex");
	renderObject->AddTexture(texturePack.bTex, "bTex");
	renderObject->AddTexture(texturePack.bgTex, "bgTex");
	renderObject->AddTexture(texturePack.splatMap, "splatMapTex");

	//boundingVolume = physicsCommon.createHeightFieldShape(/* IMPORTANT: set params*/);
	/*reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	rigidBody->updateMassPropertiesFromColliders();*/
}

Terrain::~Terrain(void) {
	//physicsCommon.destroyHeightFieldShape(boundingVolume);
}
