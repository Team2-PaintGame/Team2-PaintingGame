#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"

using namespace NCL;

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size): GameObject(physicsCommon, physicsWorld) {
	name = "BasePlayer";
	transform
		.SetScale(Vector3(size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);
	renderObject->AddTexture(0, texture, "mainTex");

	boundingVolume = physicsCommon.createBoxShape(rp3d::Vector3(size, size, size));

	reactphysics3d::Transform rp3d_transform(rp3d::Vector3(position.x, position.y, position.x), rp3d::Quaternion::identity());
	
	// Create a rigid body in the physics world
	collisionBody = physicsWorld->createRigidBody(rp3d_transform);
	collisionBody->addCollider(boundingVolume, rp3d_transform); //collider
	dynamic_cast<rp3d::RigidBody*>(collisionBody)->updateMassPropertiesFromColliders();

	//in case of material
	/*int meshLayers = mesh->GetSubMeshCount();
	for (int i = 0; i < meshLayers; i++) {
		renderObject->AddTexture(i, material->GetMaterialForLayer(i)->GetEntry("Diffuse"), "mainTex");
	}*/
}

void PlayerBase::Update(float dt) {
	
}

PlayerBase::~PlayerBase() {
	if (collisionBody) {
		physicsWorld->destroyRigidBody(dynamic_cast<rp3d::RigidBody*>(collisionBody));
	}
	physicsCommon.destroyBoxShape(boundingVolume);
}



