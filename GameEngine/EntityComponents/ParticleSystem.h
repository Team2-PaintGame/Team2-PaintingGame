#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Utils.h"
#include "Debug.h"
#include "Particle.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	class Emitter {
	public:
		Emitter(MeshGeometry* emitterMesh);
		Emitter(float angle = 20.0f);
		Vector3 GetEmissionDirection();
		unsigned int GetParticleEmissionRate() {
			return particleEmissionRate;
		}
		void SetParticleEmissionRate(int rate) {
			this->particleEmissionRate = rate;
		}
		void SetTransform(Transform* transform) {
			this->transform = transform;
		}
	private:
		MeshGeometry* emitterMesh = NULL;
		std::vector<Vector3> emissionDirections;
		unsigned int particleEmissionRate = 2;	// particles per second
		std::vector<Vector3>::iterator vIter;
		Transform* transform = NULL;
		float angle = 20.0f;
	};

	template <class T> class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity = false, float startSize = 1.0f, float startLifetime = 4.0f, float startSpeed = 10.0f, std::string name = "Particle System");
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, float emissionAngle, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity = false, float startSize = 1.0f, float startLifetime = 4.0f, float startSpeed = 10.0f, std::string name = "Particle System");
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, Vector4 color, ShaderBase* shader, bool enableGravity = false, float startSize = 1.0f, float startLifetime = 4.0f, float startSpeed = 10.0f, std::string name = "Particle System");
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, float emissionAngle, MeshGeometry* mesh, Vector4 color, ShaderBase* shader, bool enableGravity = false, float startSize = 1.0f, float startLifetime = 4.0f, float startSpeed = 10.0f, std::string name = "Particle System");
		void SetParticleEmissionRate(int rate) {
			this->emitter.SetParticleEmissionRate(rate);
		}
		void SetRunTime(float duration) {
			this->duration = duration;
		}
		virtual void Update(float dt);
		virtual ~ParticleSystem();
		void SetLooping(bool val) { looping = val; }
	protected:
		void SetMemberVariables(Vector3 emitterPosition, MeshGeometry* mesh, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, float yaw = 0.0f, float pitch = -90);
		void GenerateParticles();
		unsigned int maxParticles = 100;
		float accumulator = 0.0f;
		std::vector<std::unique_ptr<Particle>> particles;
		std::vector<Transform*> transforms;
		float elapsedTime = 0.0f;
		float duration = 15.0f;					// The length of time in seconds for the particle system to run
		bool looping = false;					// If looping is enabled, the cycle starts once again when the duration time is reached
		float startLifetime = 5.0f;				// The initial lifetime in seconds for the particles. The particle is destroyed after this elapsed time.
		float startSpeed = 10.0f;				// The initial speed of the particles. The greater the speed of the particles, the more spread out they will be.
		bool enableGravity = false;
		Emitter emitter;
	};

	template <class T>
	inline ParticleSystem<T>::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emitterMesh) {
		SetMemberVariables(emitterPosition, mesh, shader, enableGravity, startSize, startLifetime, startSpeed);
		renderObject->AddTexture(texture);
	}

	template <class T>
	inline ParticleSystem<T>::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, float emissionAngle, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emissionAngle) {
		SetMemberVariables(emitterPosition, mesh, shader, enableGravity, startSize, startLifetime, startSpeed);
		renderObject->AddTexture(texture);
	}

	template<class T>
	inline NCL::ParticleSystem<T>::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, MeshGeometry* emitterMesh, MeshGeometry* mesh, Vector4 color, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emitterMesh) {
		SetMemberVariables(emitterPosition, mesh, shader, enableGravity, startSize, startLifetime, startSpeed);
		renderObject->SetColour(color);
	}

	template<class T>
	inline NCL::ParticleSystem<T>::ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 emitterPosition, float emissionAngle, MeshGeometry* mesh, Vector4 color, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, std::string name) : GameObject(physicsCommon, physicsWorld, name), emitter(emissionAngle) {
		SetMemberVariables(emitterPosition, mesh, shader, enableGravity, startSize, startLifetime, startSpeed);
		renderObject->SetColour(color);
	}

	template <class T>
	inline void ParticleSystem<T>::Update(float dt) {
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
	inline ParticleSystem<T>::~ParticleSystem() {
		particles.clear();
	}

	template <class T>
	inline void ParticleSystem<T>::GenerateParticles() {
		if (elapsedTime < duration || looping) {
			while (accumulator > 1.0 / emitter.GetParticleEmissionRate() && particles.size() < maxParticles) {
				particles.emplace_back(std::make_unique<T>(physicsCommon, physicsWorld, transform, Vector3(), startLifetime, startSpeed, emitter.GetEmissionDirection(), enableGravity));
				transforms.emplace_back(&particles.back()->GetTransform());
				accumulator -= 1.0 / emitter.GetParticleEmissionRate();
			}
		}
	}

	template<class T>
	inline void ParticleSystem<T>::SetMemberVariables(Vector3 emitterPosition, MeshGeometry* mesh, ShaderBase* shader, bool enableGravity, float startSize, float startLifetime, float startSpeed, float yaw, float pitch) {
		this->startLifetime = startLifetime;
		this->startSpeed = startSpeed;
		this->enableGravity = enableGravity;

		transform.SetPosition(emitterPosition);
		transform.SetScale(Vector3(startSize));
		transform.SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), -90) * Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), yaw));

		emitter.SetTransform(&transform);
		//transform.SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(1, 0, 0), 45) * Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), 0));

		renderObject = new RenderObject(transforms, mesh, shader);
		renderObject->SetInstanceCount(particles.size());
		renderObject->SetIsInstanced(true);
	}
}

