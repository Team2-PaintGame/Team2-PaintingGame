#include "Ink.h"

using namespace NCL;

InkParticle::InkParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity) : Particle(physicsCommon, physicsWorld, emitterTransform, particlePosition, lifeSpan, speed, direction, enableGravity) {
	boundingVolume = physicsCommon.createSphereShape(emitterTransform.GetScale().x);
	reactphysics3d::Transform rp3d_transform(~transform.GetPosition(), ~transform.GetOrientation());

	rigidBody = physicsWorld->createRigidBody(rp3d_transform);
	rp3d::Collider* collider = rigidBody->addCollider(boundingVolume, rp3d::Transform::identity());
	collider->setIsTrigger(true);
	rigidBody->enableGravity(enableGravity);
}

InkParticle::~InkParticle() {
	if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}
	physicsCommon.destroySphereShape(dynamic_cast<rp3d::SphereShape*>(boundingVolume));
}
