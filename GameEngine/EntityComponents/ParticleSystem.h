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
		Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, Vector3 particlePosition, float size, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		void Update(float dt);
		virtual ~Particle();
	
	protected:
		float speed;
		float lifeSpan;
		Vector3 direction;
		float elapsedTime = 0.0f;
		rp3d::SphereShape* boundingVolume;
	};

	class Emitter {
	public:
		Emitter(MeshGeometry* emitterMesh) {
			this->emitterMesh = emitterMesh;
			emissionDirections = emitterMesh->GetNormalData();
			vIter = emissionDirections.begin();
		}
		Vector3 GetEmissionDirection() {
			Vector3 dir = *vIter;
			vIter = vIter + 1 == emissionDirections.end() ? emissionDirections.begin() : vIter + 1;
			return dir;
		}
		unsigned int GetParticleEmissionRate() {
			return particleEmissionRate;
		}
	private:
		MeshGeometry* emitterMesh;
		std::vector<Vector3> emissionDirections;
		unsigned int particleEmissionRate = 2;	// particles per second
		std::vector<Vector3>::iterator vIter;
	};
	
	class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity = false, float startSize = 1, float startLifetime = 4.0f, float startSpeed = 10.0f, std::string name = "Particle System");
		virtual void Update(float dt);		
		virtual ~ParticleSystem();
		
	protected:
		void GenerateParticles();
		unsigned int maxParticles = 100;
		float accumulator = 0.0f;
		std::vector<Particle*> particles;
		std::vector<Transform*> transforms;
		//float duration = 15.0f;					// The length of time in seconds for the particle system to run
		//bool looping = true;					// If looping is enabled, the cycle starts once again when the duration time is reached
		float startLifetime = 5.0f;				// The initial lifetime in seconds for the particles. The particle is destroyed after this elapsed time.
		float startSpeed = 10.0f;				// The initial speed of the particles. The greater the speed of the particles, the more spread out they will be.
		float startSize = 1.0f;					// the initial size of the particles.
		bool enableGravity = false;
		Emitter emitter;
	};
}