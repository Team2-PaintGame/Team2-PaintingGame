#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Utils.h"
#include "Matrix4.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	class Particle {
	public:
		Particle(int size) {
			Vector3 position = Vector3(rand() % 50, 10, rand() % 50);

			transform
				.SetPosition(position)
				.SetScale(Vector3(size));
		}

		void Update(float dt) {
			elapsedTime += dt;
		}
		Transform& GetTransform() { return transform; }
	protected:
		Transform transform;
		Vector3 velocity;
		float lifeSpan = 3.0f;
		float elapsedTime = 0.0f;
	};
	class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size = 1, std::string name = "Particle System");
		
		void Update(float dt) {
			for (auto& p : particles) {
				p.Update(dt);
			}
		}
		
		virtual ~ParticleSystem() {
			//physicsCommon.destroyBoxShape(boundingVolume);
		}
		
	protected:
		unsigned int numParticles = 100;
		std::vector<Particle> particles;
		rp3d::BoxShape* boundingVolume;
	};
}