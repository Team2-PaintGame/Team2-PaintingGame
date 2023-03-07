#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"
#include "Utils.h"


using namespace NCL;

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon & physicsCommon, reactphysics3d::PhysicsWorld * physicsWorld, Vector3 position, MeshGeometry * mesh, TextureBase * texture, ShaderBase * shader, int size) : GameObject(physicsCommon, physicsWorld, "BasePlayer") {
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
	physicsCommon.destroyBoxShape(dynamic_cast<rp3d::BoxShape*>(boundingVolume));

	delete camera;
}

void PlayerBase::Update(float dt) {
	camera->Update(dt);
}

void PlayerBase::SetYawPitch(float dx, float dy) {
	yaw -= dx; 
	pitch -= dy;

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
}

void PlayerBase::SetMemberVariables(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, ShaderBase* shader, int size) {
	transform
		.SetScale(Vector3(size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);

	//boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	boundingVolume = physicsCommon.createCapsuleShape(size*.35f,size);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);
  
	camera = new Camera();
}




