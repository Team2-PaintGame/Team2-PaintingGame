#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"
#include "Utils.h"


using namespace NCL;

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size): GameObject(physicsCommon, physicsWorld, "BasePlayer") {
	SetMemberVariables(physicsCommon, physicsWorld, position, mesh, shader, size);
	renderObject->SetDefaultTexture(texture);
}

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, int size) : GameObject(physicsCommon, physicsWorld, "BasePlayer") {
	SetMemberVariables(physicsCommon, physicsWorld, position, mesh, shader, size);
	renderObject->LoadMaterialTextures(meshMaterial);
}

PlayerBase::~PlayerBase() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}

	physicsCommon.destroyBoxShape(boundingVolume);

	delete camera;
}

void PlayerBase::SetMemberVariables(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, ShaderBase* shader, int size) {
	transform
		.SetScale(Vector3(size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);

	boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);

	camera = new Camera();
}



