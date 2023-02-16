#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Utils.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	//making particle's constructor protected so it can't be instantiated on its own
	class Particle : public GameObject {
	public:
		void Update(float dt);
		virtual ~Particle() {}
	
	protected:
		Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Transform emitterTransform, Vector3 particlePosition, float lifeSpan, float speed, Vector3 direction, bool enableGravity);
		float speed;
		float lifeSpan;
		Vector3 direction;
		float elapsedTime = 0.0f;
	};

	class Emitter {
	public:
		Emitter(MeshGeometry* emitterMesh);
		Vector3 GetEmissionDirection();
		unsigned int GetParticleEmissionRate() {
			return particleEmissionRate;
		}
	private:
		MeshGeometry* emitterMesh;
		std::vector<Vector3> emissionDirections;
		unsigned int particleEmissionRate = 2;	// particles per second
		std::vector<Vector3>::iterator vIter;
	};
	
	template <class T> class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity = false, float startSize = 1, float startLifetime = 4.0f, float startSpeed = 10.0f, std::string name = "Particle System");
		virtual void Update(float dt);		
		virtual ~ParticleSystem();
		
	protected:
		void GenerateParticles();
		unsigned int maxParticles = 100;
		float accumulator = 0.0f;
		std::vector<std::unique_ptr<Particle>> particles;
		std::vector<Transform*> transforms;
		float elapsedTime = 0.0f;
		float duration = 15.0f;					// The length of time in seconds for the particle system to run
		bool looping = true;					// If looping is enabled, the cycle starts once again when the duration time is reached
		float startLifetime = 5.0f;				// The initial lifetime in seconds for the particles. The particle is destroyed after this elapsed time.
		float startSpeed = 10.0f;				// The initial speed of the particles. The greater the speed of the particles, the more spread out they will be.
		bool enableGravity = false;
		Emitter emitter;
	};

	template <class T>
	ParticleSystem<T>::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emitterMesh) {
		this->startLifetime = startLifetime;
		this->startSpeed = startSpeed;
		this->enableGravity = enableGravity;

		transform.SetPosition(emitterPosition);
		transform.SetScale(startSize);
		//transform.SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 45) * Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), 0));

		renderObject = new RenderObject(transforms, mesh, shader);
		renderObject->AddTexture(texture);
		renderObject->SetInstanceCount(particles.size());
		renderObject->SetIsInstanced(true);
	}

	template <class T>
	void ParticleSystem<T>::Update(float dt) {
		accumulator += dt;
		elapsedTime += dt;
		GenerateParticles();

		for (size_t i = 0; i < particles.size();) {
			particles[i]->Update(dt);

			if (!particles[i]->IsActive()) {
				std::swap(particles[i], particles.back());
				particles.pop_back();
				std::swap(transforms[i], transforms.back());
				transforms.pop_back();
			}
			else
				++i;
		}

		renderObject->SetTransforms(transforms);
		renderObject->SetInstanceCount(particles.size());
	}

	template <class T>
	ParticleSystem<T>::~ParticleSystem() {
		particles.clear();
	}

	template <class T>
	void ParticleSystem<T>::GenerateParticles() {
		if (elapsedTime < duration || looping) {
			while (accumulator > 1.0 / emitter.GetParticleEmissionRate() && particles.size() < maxParticles) {
				particles.emplace_back(std::make_unique<T>(physicsCommon, physicsWorld, transform, Vector3(), startLifetime, startSpeed, emitter.GetEmissionDirection(), enableGravity));
				transforms.emplace_back(&particles.back()->GetTransform());
				accumulator -= 1.0 / emitter.GetParticleEmissionRate();
			}
		}
	}
}

