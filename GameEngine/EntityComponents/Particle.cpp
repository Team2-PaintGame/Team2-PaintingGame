#include "ParticleSystem.h"

using namespace NCL;

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, Vector4 colour, bool enableGravity, Layer layer) : GameObject(physicsCommon, physicsWorld, "Particle") {
	transform
		.SetPosition(emitterTransform->GetPosition() + particlePosition)
		.SetScale(emitterTransform->GetScale())
		.SetOrientation(emitterTransform->GetOrientation());
	
	this->lifeSpan = lifeSpan;
	this->speed = speed;
	this->direction = direction;
	this->particleColour = colour;
	//layer = Layer::Paint;
	this->layer = layer;
}

void Particle::Update(float dt) {
	elapsedTime += dt;

	rigidBody->applyWorldForceAtCenterOfMass(~( direction * speed));
	
	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	//Debug::DrawLine(transform.GetPosition(), transform.GetPosition() - transform.GetMatrix().GetColumn(2), transform.GetMatrix().GetColumn(2), 0.5f);

	if (elapsedTime >= lifeSpan) {
		this->SetActive(false);
	}
}


CapsuleParticle::CapsuleParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity, Layer layer) : Particle(physicsCommon, physicsWorld, emitterTransform, particlePosition, lifeSpan, speed, direction, {0,0,0,}, enableGravity, layer) {
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
	physicsCommon.destroyCapsuleShape((rp3d::CapsuleShape*)(boundingVolume));
}

SphereParticle::SphereParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, Vector4 colour, bool enableGravity, Layer layer) : Particle(physicsCommon, physicsWorld, emitterTransform, particlePosition, lifeSpan, speed, direction, colour, enableGravity, layer) {
	boundingVolume = physicsCommon.createSphereShape(transform.GetScale().x / 2.0f);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), ~transform.GetOrientation());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rp3d::Collider* collider = rigidBody->addCollider(boundingVolume, rp3d::Transform::identity());
	collider->setIsTrigger(true);
	rigidBody->enableGravity(enableGravity);
	rigidBody->setMass(1.f); 
	rigidBody->setUserData(this);
}

SphereParticle::~SphereParticle() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}
	physicsCommon.destroySphereShape((rp3d::SphereShape*)(boundingVolume));
}
