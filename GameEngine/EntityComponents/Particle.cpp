#include "ParticleSystem.h"

using namespace NCL;

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity) : GameObject(physicsCommon, physicsWorld) {
	transform
		.SetPosition(emitterTransform->GetPosition() + particlePosition)
		.SetScale(emitterTransform->GetScale())
		.SetOrientation(emitterTransform->GetOrientation());
	
	this->lifeSpan = lifeSpan;
	this->speed = speed;
	this->direction = direction;
}

void Particle::Update(float dt) {
	elapsedTime += dt;

	rigidBody->applyWorldForceAtCenterOfMass(~(transform.GetOrientation() * direction * speed));
	
	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	if (elapsedTime >= lifeSpan) {
		this->SetActive(false);
	}
}


CapsuleParticle::CapsuleParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity) : Particle(physicsCommon, physicsWorld, emitterTransform, particlePosition, lifeSpan, speed, direction, enableGravity) {
	boundingVolume = physicsCommon.createCapsuleShape(transform.GetScale().x / 2.0f, transform.GetScale().x);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), ~transform.GetOrientation());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rp3d::Collider* collider = rigidBody->addCollider(boundingVolume, rp3d::Transform::identity());
	collider->setIsTrigger(true);
	rigidBody->enableGravity(enableGravity);
}

CapsuleParticle::~CapsuleParticle() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}
	physicsCommon.destroyCapsuleShape(dynamic_cast<rp3d::CapsuleShape*>(boundingVolume));
}

SphereParticle::SphereParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity) : Particle(physicsCommon, physicsWorld, emitterTransform, particlePosition, lifeSpan, speed, direction, enableGravity) {
	boundingVolume = physicsCommon.createSphereShape(transform.GetScale().x / 2.0f);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), ~transform.GetOrientation());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rp3d::Collider* collider = rigidBody->addCollider(boundingVolume, rp3d::Transform::identity());
	collider->setIsTrigger(true);
	rigidBody->enableGravity(enableGravity);
}

SphereParticle::~SphereParticle() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}
	physicsCommon.destroySphereShape(dynamic_cast<rp3d::SphereShape*>(boundingVolume));
}
