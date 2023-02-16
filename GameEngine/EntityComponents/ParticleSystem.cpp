#include "ParticleSystem.h"
#include "Utils.h"

using namespace NCL;

Particle::Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity) : GameObject(physicsCommon, physicsWorld) {
	transform
		.SetPosition(emitterTransform.GetPosition() + particlePosition)
		.SetScale(emitterTransform.GetScale())
		.SetOrientation(emitterTransform.GetOrientation());
	
	this->lifeSpan = lifeSpan;
	this->speed = speed;
	this->direction = direction;
}

void Particle::Update(float dt) {
	elapsedTime += dt;

	//need to call this here because particles itself are not part of game world
	GameObject::UpdateTransform();

	rigidBody->applyWorldForceAtCenterOfMass(~(transform.GetOrientation() * direction * speed));

	if (elapsedTime >= lifeSpan) {
		this->SetActive(false);
	}
}

Emitter::Emitter(MeshGeometry* emitterMesh) {
	this->emitterMesh = emitterMesh;
	emissionDirections = emitterMesh->GetNormalData();
	vIter = emissionDirections.begin();
}

Emitter::Emitter(float angle) {
	this->angle = angle;
}

Vector3 Emitter::GetEmissionDirection() {
	if (emitterMesh) {
		Vector3 dir = *vIter;
		vIter = vIter + 1 == emissionDirections.end() ? emissionDirections.begin() : vIter + 1;
		return dir;
	}
	else {
		float pitch = fmod(rand(), (2.0f * angle+ 1) - angle);
		float yaw = fmod(rand(), (2.0f * angle + 1) - angle);
		return Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), pitch) * Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), yaw) * transform->GetPosition();
	}
}
