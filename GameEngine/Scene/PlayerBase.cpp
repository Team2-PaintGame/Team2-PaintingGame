#include "PlayerBase.h"
#include "RenderObject.h"
#include "Window.h"
#include "Utils.h"


using namespace NCL;

PlayerBase::PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size): GameObject(physicsCommon, physicsWorld, "BasePlayer") {
	transform
		.SetScale(Vector3(size))
		.SetPosition(position);

	renderObject = new RenderObject(&transform, mesh, shader);
	renderObject->AddTexture(texture);

	boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());
	
	// Create a rigid body in the physics world
	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
	rigidBody->updateMassPropertiesFromColliders();
	rigidBody->setLinearDamping(1.5f);
}

void PlayerBase::Update(float dt) {
}

PlayerBase::~PlayerBase() {
	physicsCommon.destroyBoxShape(boundingVolume); 
}

/*
void PlayerBase::ShootBullet() {

	timeSinceLastShot = 0.0f;

	GameObject* bullet = new Bullet(this->GetTransform().GetPosition(), meshes.at("sphereMesh"), textures.at("basicTex"), shaders.at("basicShader"), 1);

	reactphysics3d::SphereShape* bulletShape = physicsCommon.createSphereShape(0.5f);
	reactphysics3d::Transform rp3d_transform(~player->GetTransform().GetPosition(), rp3d::Quaternion::identity());
	reactphysics3d::RigidBody* bulletRigidBody = physicsWorld->createRigidBody(rp3d_transform);
	bulletRigidBody->addCollider(bulletShape, rp3d::Transform::identity());
	bulletRigidBody->updateMassPropertiesFromColliders();

	Vector3 force = gunDirection * forceMagnitude;
	bulletRigidBody->setLinearVelocity(~force);

	world->AddGameObject(bullet);
	*/
	/*
	timeSinceLastShot = 0.0f;
	reactphysics3d::RigidBody* bullet = physicsWorld->createRigidBody(reactphysics3d::Transform::identity());

	reactphysics3d::SphereShape* bulletShape = physicsCommon.createSphereShape(0.5f);
	bullet->addCollider(bulletShape, rp3d::Transform::identity());
	bullet->updateMassPropertiesFromColliders();

	rp3d::Quaternion orientation = rigidBody->getTransform().getOrientation();
	rp3d::Vector3 forward = orientation * rp3d::Vector3(1, 0, 0);
	bullet->setLinearVelocity(forward * 100000.0f);

	}
	*/
	






