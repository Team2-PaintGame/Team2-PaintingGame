#pragma once
#include "Vector3.h"
#include "GameObject.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	//making particle's constructor protected so it can't be instantiated on its own
	class Particle : public GameObject {
	public:
		void Update(float dt);
		virtual ~Particle() {}
	
	protected:
		Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		float speed;
		float lifeSpan;
		Vector3 direction;
		float elapsedTime = 0.0f;
	};


	class CapsuleParticle : public Particle {
	public:
		CapsuleParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		virtual ~CapsuleParticle();
	};

	class SphereParticle : public Particle {
	public:
		SphereParticle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform* emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		virtual ~SphereParticle();
	};
}

