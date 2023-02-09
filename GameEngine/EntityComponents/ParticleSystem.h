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
		Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, int size);
		void Update(float dt);
		virtual ~Particle();
	
	protected:
		Vector3 velocity;
		float lifeSpan = 3.0f;
		float elapsedTime = 0.0f;
		rp3d::BoxShape* boundingVolume;
	};
	
	class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size = 1, std::string name = "Particle System");
		void Update(float dt);		
		virtual ~ParticleSystem();
		
	protected:
		unsigned int numParticles = 100;
		std::vector<Particle*> particles;
	};
}