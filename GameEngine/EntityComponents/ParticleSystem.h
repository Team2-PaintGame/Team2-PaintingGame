#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Utils.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	class Particle : public GameObject {
	public:
		Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, int size, float lifeSpan);
		void Update(float dt);
		virtual ~Particle();
	
	protected:
		float speed = 10.0f;
		Vector3 direction = Vector3(1, 0, 0);
		float lifeSpan = 3.0f;
		float elapsedTime = 0.0f;
		rp3d::SphereShape* boundingVolume;
	};
	
	class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int startSize = 1, float startLifetime = 4.0f, std::string name = "Particle System");
		virtual void Update(float dt);		
		virtual ~ParticleSystem();
		
	protected:
		unsigned int numParticles = 14;
		std::vector<Particle*> particles;
		std::vector<Transform*> transforms;
		float duration = 5.0f;			// The length of time in seconds for the particle system to run
		bool looping = true;			// If looping is enabled, the cycle starts once again when the duration time is reached
		float startLifetime = 4.0f;		// The initial lifetime in seconds for the particles. The particle is destroyed after this elapsed time.
		float startSpeed = 1.0f;		// The initial speed of the particles. The greater the speed of the particles, the more spread out they will be.
		float startSize = 1.0f;			// the initial size of the particles.
	};
}